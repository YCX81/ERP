#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerdialog.h"
#include "logindialog.h"
#include "materialdialog.h"
#include "defectivedialog.h"
#include "bomdialog.h"
#include "orderdialog.h"
#include "supplierdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QTabWidget>
#include <QRegularExpression>
#include <QDateTime>
#include <QHeaderView>
#include <QApplication>
#include <QStyledItemDelegate>
#include <QFont>
#include <xlsxdocument.h>
#include <QUndoStack>
#include <QUndoCommand>

// 创建一个委托，用于设置单元格的对齐方式
class AlignmentDelegate : public QStyledItemDelegate {
public:
    explicit AlignmentDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    // 重写 paint 函数，设置对齐方式
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {
        QStyleOptionViewItem alignedOption = option;
        QVariant value = index.data(Qt::DisplayRole);
        if (value.type() == QVariant::Int || value.type() == QVariant::Double) {
            alignedOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
        } else {
            alignedOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
        }
        QStyledItemDelegate::paint(painter, alignedOption, index);
    }
};

// Implement AddMaterialCommand
AddMaterialCommand::AddMaterialCommand(QSqlTableModel *model, const QSqlRecord &record, QUndoCommand *parent)
    : QUndoCommand(parent), model(model), record(record), row(-1)
{
    setText("添加物料");
}

void AddMaterialCommand::undo()
{
    if (row >= 0)
    {
        model->removeRow(row);
        if (!model->submitAll()) {
            QMessageBox::critical(nullptr, "错误", "无法撤销添加物料：" + model->lastError().text());
        }
    }
}

void AddMaterialCommand::redo()
{
    if (row == -1) { // 第一次执行 redo
        row = model->rowCount();
        model->insertRecord(row, record);
    }
    if (!model->submitAll()) {
        QMessageBox::critical(nullptr, "错误", "无法添加物料：" + model->lastError().text());
    }
}

// Implement EditMaterialCommand
EditMaterialCommand::EditMaterialCommand(QSqlTableModel *model, int row, const QSqlRecord &oldRecord, const QSqlRecord &newRecord, QUndoCommand *parent)
    : QUndoCommand(parent), model(model), row(row), oldRecord(oldRecord), newRecord(newRecord)
{
    setText("编辑物料");
}

void EditMaterialCommand::undo()
{
    model->setRecord(row, oldRecord);
    if (!model->submitAll()) {
        QMessageBox::critical(nullptr, "错误", "无法撤销编辑物料：" + model->lastError().text());
    }
}

void EditMaterialCommand::redo()
{
    model->setRecord(row, newRecord);
    if (!model->submitAll()) {
        QMessageBox::critical(nullptr, "错误", "无法重新编辑物料：" + model->lastError().text());
    }
}

// Implement DeleteMaterialCommand
DeleteMaterialCommand::DeleteMaterialCommand(QSqlTableModel *model, int row, const QSqlRecord &record, QUndoCommand *parent)
    : QUndoCommand(parent), model(model), row(row), record(record)
{
    setText("删除物料");
}

void DeleteMaterialCommand::undo()
{
    model->insertRecord(row, record);
    if (!model->submitAll()) {
        QMessageBox::critical(nullptr, "错误", "无法撤销删除物料：" + model->lastError().text());
    }
}

void DeleteMaterialCommand::redo()
{
    model->removeRow(row);
    if (!model->submitAll()) {
        QMessageBox::critical(nullptr, "错误", "无法删除物料：" + model->lastError().text());
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , materialModel(new QSqlTableModel(this))
    , defectiveModel(new QSqlTableModel(this))
    , bomModel(new QSqlTableModel(this))
    , supplierModel(new QSqlTableModel(this))
    , materialProxyModel(new QSortFilterProxyModel(this))
    , defectiveProxyModel(new QSortFilterProxyModel(this))
    , bomProxyModel(new QSortFilterProxyModel(this))
    , supplierProxyModel(new QSortFilterProxyModel(this))
    , undoStack(new QUndoStack(this))
{
    ui->setupUi(this);

    // 连接撤销和重做按钮
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::on_undoButton_clicked);
    connect(ui->redoButton, &QPushButton::clicked, this, &MainWindow::on_redoButton_clicked);

    // 设置全局样式表，统一界面风格
    qApp->setStyleSheet(
        "QTableView {"
        "    gridline-color: #dcdcdc;"
        "}"
        "QHeaderView::section {"
        "    background-color: #f0f0f0;"
        "    border: 1px solid #dcdcdc;"
        "    padding: 4px;"
        "    font-weight: bold;"
        "}"
        );

    // 设置字体
    QFont tableFont("Microsoft YaHei", 10);

    // 初始化供应商模型
    supplierModel->setTable("Suppliers");
    supplierModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    supplierModel->select();

    // 设置表头名称（翻译为中文）
    supplierModel->setHeaderData(supplierModel->fieldIndex("supplier_id"), Qt::Horizontal, "供应商代码");
    supplierModel->setHeaderData(supplierModel->fieldIndex("supplier_name"), Qt::Horizontal, "供应商名称");
    supplierModel->setHeaderData(supplierModel->fieldIndex("business_nature"), Qt::Horizontal, "经营性质");
    supplierModel->setHeaderData(supplierModel->fieldIndex("primary_contact"), Qt::Horizontal, "第一联系人");
    supplierModel->setHeaderData(supplierModel->fieldIndex("primary_contact_phone"), Qt::Horizontal, "第一联系人电话");
    supplierModel->setHeaderData(supplierModel->fieldIndex("secondary_contact"), Qt::Horizontal, "第二联系人");
    supplierModel->setHeaderData(supplierModel->fieldIndex("secondary_contact_phone"), Qt::Horizontal, "第二联系人电话");
    supplierModel->setHeaderData(supplierModel->fieldIndex("address"), Qt::Horizontal, "地址");
    supplierModel->setHeaderData(supplierModel->fieldIndex("supplier_manager"), Qt::Horizontal, "供应商负责人");

    // 初始化代理模型用于供应商搜索过滤
    supplierProxyModel->setSourceModel(supplierModel);
    supplierProxyModel->setFilterKeyColumn(-1); // 设置为所有列
    supplierProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // 设置供应商表格视图
    ui->supplierTableView->setModel(supplierProxyModel);
    ui->supplierTableView->setItemDelegate(new AlignmentDelegate(this)); // 设置委托
    ui->supplierTableView->setFont(tableFont);
    ui->supplierTableView->verticalHeader()->setDefaultSectionSize(24);
    ui->supplierTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 自动调整列宽
    ui->supplierTableView->horizontalHeader()->setStretchLastSection(true);
    ui->supplierTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->supplierTableView->setSelectionMode(QAbstractItemView::ExtendedSelection); // 允许多行选择
    ui->supplierTableView->setSortingEnabled(true);
    ui->supplierTableView->setColumnHidden(supplierModel->fieldIndex("id"), true); // 隐藏不需要的列
    ui->supplierTableView->setColumnHidden(supplierModel->fieldIndex("supplier_id"), true); // 隐藏 supplier_id 列

    // 初始化物料模型
    materialModel->setTable("Material");
    materialModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    materialModel->select();

    // 设置表头名称（翻译为中文）
    materialModel->setHeaderData(materialModel->fieldIndex("material_number"), Qt::Horizontal, "物料号");
    materialModel->setHeaderData(materialModel->fieldIndex("description"), Qt::Horizontal, "描述");
    materialModel->setHeaderData(materialModel->fieldIndex("remarks"), Qt::Horizontal, "备注");
    materialModel->setHeaderData(materialModel->fieldIndex("self_made_or_purchase"), Qt::Horizontal, "自制或采购");
    materialModel->setHeaderData(materialModel->fieldIndex("serial_number"), Qt::Horizontal, "序列号");
    materialModel->setHeaderData(materialModel->fieldIndex("location_number"), Qt::Horizontal, "库位号");
    materialModel->setHeaderData(materialModel->fieldIndex("version"), Qt::Horizontal, "版本号");
    materialModel->setHeaderData(materialModel->fieldIndex("delivery_period"), Qt::Horizontal, "货期（天）");
    materialModel->setHeaderData(materialModel->fieldIndex("created_at"), Qt::Horizontal, "创建日期");
    materialModel->setHeaderData(materialModel->fieldIndex("updated_at"), Qt::Horizontal, "更新日期");
    materialModel->setHeaderData(materialModel->fieldIndex("unit_price"), Qt::Horizontal, "单价");
    materialModel->setHeaderData(materialModel->fieldIndex("maintainer"), Qt::Horizontal, "维护人");
    materialModel->setHeaderData(materialModel->fieldIndex("good_quantity"), Qt::Horizontal, "良品数量");
    materialModel->setHeaderData(materialModel->fieldIndex("defective_quantity"), Qt::Horizontal, "不良品数量");
    materialModel->setHeaderData(materialModel->fieldIndex("category_name"), Qt::Horizontal, "类别");
    materialModel->setHeaderData(materialModel->fieldIndex("supplier_name"), Qt::Horizontal, "供应商名称");

    // 初始化代理模型用于物料搜索和排序
    materialProxyModel->setSourceModel(materialModel);
    materialProxyModel->setFilterKeyColumn(-1); // 所有列
    materialProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // 设置物料表格视图
    ui->materialTableView->setModel(materialProxyModel);
    ui->materialTableView->setItemDelegate(new AlignmentDelegate(this)); // 设置委托
    ui->materialTableView->setFont(tableFont);
    ui->materialTableView->verticalHeader()->setDefaultSectionSize(24);
    ui->materialTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 自动调整列宽
    ui->materialTableView->horizontalHeader()->setStretchLastSection(true);
    ui->materialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->materialTableView->setSelectionMode(QAbstractItemView::ExtendedSelection); // 允许多行选择
    ui->materialTableView->setSortingEnabled(true);
    ui->materialTableView->setColumnHidden(materialModel->fieldIndex("id"), true); // 隐藏不需要的列
    ui->materialTableView->setColumnHidden(materialModel->fieldIndex("drawing"), true);
    ui->materialTableView->setColumnHidden(materialModel->fieldIndex("photo"), true);
    ui->materialTableView->setColumnHidden(materialModel->fieldIndex("category_code"), true); // 隐藏 category_code 列
    ui->materialTableView->setColumnHidden(materialModel->fieldIndex("mk_number"), true);     // 隐藏 mk_number 列
    ui->materialTableView->setColumnHidden(materialModel->fieldIndex("supplier_id"), true);   // 隐藏 supplier_id 列

    // 调整列宽，避免过宽或过窄
    ui->materialTableView->horizontalHeader()->setDefaultSectionSize(100); // 设置默认列宽
    ui->materialTableView->horizontalHeader()->setMinimumSectionSize(50);  // 设置最小列宽

    // 初始化不良品模型
    defectiveModel->setTable("Material");
    defectiveModel->setFilter("defective_quantity > 0");
    defectiveModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    defectiveModel->select();

    // 设置表头名称（翻译为中文）
    defectiveModel->setHeaderData(defectiveModel->fieldIndex("material_number"), Qt::Horizontal, "物料号");
    defectiveModel->setHeaderData(defectiveModel->fieldIndex("description"), Qt::Horizontal, "描述");
    defectiveModel->setHeaderData(defectiveModel->fieldIndex("defective_quantity"), Qt::Horizontal, "不良品数量");
    defectiveModel->setHeaderData(defectiveModel->fieldIndex("maintainer"), Qt::Horizontal, "维护人");
    defectiveModel->setHeaderData(defectiveModel->fieldIndex("updated_at"), Qt::Horizontal, "更新日期");

    // 初始化代理模型用于不良品搜索和排序
    defectiveProxyModel->setSourceModel(defectiveModel);
    defectiveProxyModel->setFilterKeyColumn(-1);
    defectiveProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // 设置不良品表格视图
    ui->defectiveTableView->setModel(defectiveProxyModel);
    ui->defectiveTableView->setItemDelegate(new AlignmentDelegate(this)); // 设置委托
    ui->defectiveTableView->setFont(tableFont);
    ui->defectiveTableView->verticalHeader()->setDefaultSectionSize(24);
    ui->defectiveTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 自动调整列宽
    ui->defectiveTableView->horizontalHeader()->setStretchLastSection(true);
    ui->defectiveTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->defectiveTableView->setSelectionMode(QAbstractItemView::ExtendedSelection); // 允许多行选择
    ui->defectiveTableView->setSortingEnabled(true);
    ui->defectiveTableView->setColumnHidden(defectiveModel->fieldIndex("id"), true); // 隐藏不需要的列
    ui->defectiveTableView->setColumnHidden(defectiveModel->fieldIndex("drawing"), true);
    ui->defectiveTableView->setColumnHidden(defectiveModel->fieldIndex("photo"), true);
    ui->defectiveTableView->setColumnHidden(defectiveModel->fieldIndex("category_code"), true); // 隐藏 category_code 列
    ui->defectiveTableView->setColumnHidden(defectiveModel->fieldIndex("mk_number"), true);     // 隐藏 mk_number 列
    ui->defectiveTableView->setColumnHidden(defectiveModel->fieldIndex("supplier_id"), true);   // 隐藏 supplier_id 列

    // 初始化 BOM 模型
    bomModel->setTable("BOM");
    bomModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bomModel->select();

    // 设置表头名称（翻译为中文）
    bomModel->setHeaderData(bomModel->fieldIndex("bom_number"), Qt::Horizontal, "BOM 编号");
    bomModel->setHeaderData(bomModel->fieldIndex("bom_name"), Qt::Horizontal, "BOM 名称");
    bomModel->setHeaderData(bomModel->fieldIndex("description"), Qt::Horizontal, "描述");
    bomModel->setHeaderData(bomModel->fieldIndex("version"), Qt::Horizontal, "版本号");
    bomModel->setHeaderData(bomModel->fieldIndex("maintainer"), Qt::Horizontal, "维护人");
    bomModel->setHeaderData(bomModel->fieldIndex("updated_at"), Qt::Horizontal, "更新日期");

    // 初始化代理模型用于 BOM 搜索和排序
    bomProxyModel->setSourceModel(bomModel);
    bomProxyModel->setFilterKeyColumn(-1);
    bomProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // 设置 BOM 表格视图
    ui->bomTableView->setModel(bomProxyModel);
    ui->bomTableView->setItemDelegate(new AlignmentDelegate(this)); // 设置委托
    ui->bomTableView->setFont(tableFont);
    ui->bomTableView->verticalHeader()->setDefaultSectionSize(24);
    ui->bomTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 自动调整列宽
    ui->bomTableView->horizontalHeader()->setStretchLastSection(true);
    ui->bomTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bomTableView->setSelectionMode(QAbstractItemView::ExtendedSelection); // 允许多行选择
    ui->bomTableView->setSortingEnabled(true);
    ui->bomTableView->setColumnHidden(bomModel->fieldIndex("id"), true); // 隐藏不需要的列

    // 初始化订单模型和视图
    QStringList statuses = { "预下单", "缺料", "制作中", "制作完毕", "已出库" };
    for (const QString &status : statuses) {
        QSqlTableModel *orderModel = new QSqlTableModel(this);
        orderModel->setTable("`Order`");
        orderModel->setFilter(QString("status = '%1'").arg(status));
        orderModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        orderModel->select();

        // 设置表头名称（翻译为中文）
        orderModel->setHeaderData(orderModel->fieldIndex("order_number"), Qt::Horizontal, "订单编号");
        orderModel->setHeaderData(orderModel->fieldIndex("customer_name"), Qt::Horizontal, "客户名称");
        orderModel->setHeaderData(orderModel->fieldIndex("status"), Qt::Horizontal, "状态");
        orderModel->setHeaderData(orderModel->fieldIndex("order_date"), Qt::Horizontal, "订单日期");
        orderModel->setHeaderData(orderModel->fieldIndex("delivery_date"), Qt::Horizontal, "交付日期");
        orderModel->setHeaderData(orderModel->fieldIndex("order_manager"), Qt::Horizontal, "订单负责人");
        orderModel->setHeaderData(orderModel->fieldIndex("maintainer"), Qt::Horizontal, "维护人"); // 新增字段

        orderModels[status] = orderModel;

        QSortFilterProxyModel *orderProxyModel = new QSortFilterProxyModel(this);
        orderProxyModel->setSourceModel(orderModel);
        orderProxyModel->setFilterKeyColumn(-1);
        orderProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        orderProxyModels[status] = orderProxyModel;

        QLineEdit *searchLineEdit = new QLineEdit;
        searchLineEdit->setPlaceholderText("搜索订单...");
        connect(searchLineEdit, &QLineEdit::textChanged, [orderProxyModel](const QString &text) {
            QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
            orderProxyModel->setFilterRegularExpression(regExp);
        });

        QTableView *tableView = new QTableView;
        tableView->setModel(orderProxyModel);
        tableView->setItemDelegate(new AlignmentDelegate(this)); // 设置委托
        tableView->setFont(tableFont);
        tableView->verticalHeader()->setDefaultSectionSize(24);
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 自动调整列宽
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSelectionMode(QAbstractItemView::ExtendedSelection); // 允许多行选择
        tableView->setSortingEnabled(true);
        tableView->setColumnHidden(orderModel->fieldIndex("id"), true); // 隐藏不需要的列

        orderTableViews[status] = tableView;

        QWidget *tab = new QWidget;
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);
        tabLayout->addWidget(searchLineEdit); // 添加搜索框
        tabLayout->addWidget(tableView);
        ui->orderTabWidget->addTab(tab, status);
    }

    // 初始化供应商物料表视图
    ui->supplierMaterialTableView->setModel(nullptr);
    ui->supplierMaterialTableView->setItemDelegate(new AlignmentDelegate(this));
    ui->supplierMaterialTableView->setFont(tableFont);
    ui->supplierMaterialTableView->verticalHeader()->setDefaultSectionSize(24);
    ui->supplierMaterialTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->supplierMaterialTableView->horizontalHeader()->setStretchLastSection(true);
    ui->supplierMaterialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->supplierMaterialTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->supplierMaterialTableView->setSortingEnabled(true);

    // 初始化 BOM 物料表视图
    ui->bomMaterialTableView->setModel(nullptr);
    ui->bomMaterialTableView->setItemDelegate(new AlignmentDelegate(this));
    ui->bomMaterialTableView->setFont(tableFont);
    ui->bomMaterialTableView->verticalHeader()->setDefaultSectionSize(24);
    ui->bomMaterialTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->bomMaterialTableView->horizontalHeader()->setStretchLastSection(true);
    ui->bomMaterialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bomMaterialTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->bomMaterialTableView->setSortingEnabled(true);

    // 连接搜索框信号到槽
    connect(ui->supplierSearchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_supplierSearchLineEdit_textChanged);
    connect(ui->materialSearchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_materialSearchLineEdit_textChanged);
    connect(ui->defectiveSearchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_defectiveSearchLineEdit_textChanged);
    connect(ui->bomSearchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_bomSearchLineEdit_textChanged);

    // 连接表视图的选择变化信号到槽
    connect(ui->materialTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onMaterialTableViewSelectionChanged);
    connect(ui->supplierTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onSupplierSelectionChanged);
    connect(ui->bomTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onBOMSelectionChanged);

    // 更新登录状态
    updateLoginStatus();

    // 默认显示物料管理页面
    ui->stackedWidget->setCurrentWidget(ui->materialPage);
}

// 主窗口析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

// 用户注册槽函数
void MainWindow::on_registerButton_clicked()
{
    RegisterDialog regDialog(this);
    if (regDialog.exec() == QDialog::Accepted) {
        QMessageBox::information(this, "注册成功", "用户已成功注册。");
    }
}

// 用户登录槽函数
void MainWindow::on_loginButton_clicked()
{
    LoginDialog loginDialog(this);
    if (loginDialog.exec() == QDialog::Accepted) {
        currentUser = loginDialog.getUsername();
        updateLoginStatus();
    }
}

// 用户注销槽函数
void MainWindow::on_logoutButton_clicked()
{
    currentUser.clear();
    updateLoginStatus();
}

// 切换用户槽函数
void MainWindow::on_switchUserButton_clicked()
{
    on_loginButton_clicked();
}

// 更新登录状态的辅助函数
void MainWindow::updateLoginStatus()
{
    bool isLoggedIn = !currentUser.isEmpty();
    ui->loginButton->setVisible(!isLoggedIn);
    ui->registerButton->setVisible(!isLoggedIn);
    ui->logoutButton->setVisible(isLoggedIn);
    ui->switchUserButton->setVisible(isLoggedIn);
    ui->currentUserLabel->setVisible(isLoggedIn);
    if (isLoggedIn) {
        ui->currentUserLabel->setText(tr("当前用户：%1").arg(currentUser));
        ui->statusbar->showMessage(tr("当前用户：%1").arg(currentUser));
    } else {
        ui->currentUserLabel->setText("");
        ui->statusbar->showMessage(tr("未登录"));
    }
}

// 界面切换槽函数
void MainWindow::on_materialButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->materialPage);
}

void MainWindow::on_defectiveButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->defectivePage);
}

void MainWindow::on_bomButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->bomPage);
}

void MainWindow::on_orderButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->orderPage);
}

void MainWindow::on_supplierButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->supplierPage);
}

// 添加物料槽函数
void MainWindow::on_addButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    MaterialDialog dialog(currentUser, this);
    if (dialog.exec() == QDialog::Accepted) {
        QSqlRecord newRecord = dialog.getMaterialData();
        // 使用撤销命令
        undoStack->push(new AddMaterialCommand(materialModel, newRecord));
    }
}

// 编辑物料槽函数
void MainWindow::on_editButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的物料。");
        return;
    }

    int row = selectedRows.first().row();
    QModelIndex sourceIndex = materialProxyModel->mapToSource(materialProxyModel->index(row, 0));
    QSqlRecord oldRecord = materialModel->record(sourceIndex.row());

    MaterialDialog dialog(currentUser, this);
    dialog.setMaterialData(oldRecord);
    if (dialog.exec() == QDialog::Accepted) {
        QSqlRecord newRecord = dialog.getMaterialData();
        // 使用撤销命令
        undoStack->push(new EditMaterialCommand(materialModel, sourceIndex.row(), oldRecord, newRecord));
    }
}

// 删除物料槽函数
void MainWindow::on_deleteButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的物料。");
        return;
    }

    int ret = QMessageBox::question(this, "确认删除", "确定要删除选中的物料吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        foreach (const QModelIndex &index, selectedRows) {
            int row = index.row();
            QModelIndex sourceIndex = materialProxyModel->mapToSource(materialProxyModel->index(row, 0));
            QSqlRecord record = materialModel->record(sourceIndex.row());

            // 使用撤销命令
            undoStack->push(new DeleteMaterialCommand(materialModel, sourceIndex.row(), record));
        }
    }
}

// 导出物料到Excel槽函数
void MainWindow::on_exportButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出Excel", "", "Excel 文件 (*.xlsx)");
    if (!filePath.isEmpty()) {
        QXlsx::Document xlsxDoc;

        // 获取选中的行
        QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
        if (selectedRows.isEmpty()) {
            QMessageBox::warning(this, "警告", "请先选择要导出的物料。");
            return;
        }

        // 获取列数
        int columnCount = materialProxyModel->columnCount();

        // 写入表头
        int colOffset = 1;
        for (int col = 0; col < columnCount; ++col) {
            if (ui->materialTableView->isColumnHidden(col)) {
                continue;
            }
            QString header = materialModel->headerData(col, Qt::Horizontal).toString();
            xlsxDoc.write(1, colOffset, header);
            ++colOffset;
        }

        // 写入数据
        int rowNum = 2;
        foreach (const QModelIndex &index, selectedRows) {
            colOffset = 1;
            for (int col = 0; col < columnCount; ++col) {
                if (ui->materialTableView->isColumnHidden(col)) {
                    continue;
                }
                QVariant data = materialProxyModel->data(materialProxyModel->index(index.row(), col));
                xlsxDoc.write(rowNum, colOffset, data);
                ++colOffset;
            }
            ++rowNum;
        }

        xlsxDoc.saveAs(filePath);
        QMessageBox::information(this, "导出成功", "数据已成功导出。");
    }
}

// 查看图纸槽函数
void MainWindow::on_viewDrawingButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个物料。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = materialModel->record(materialProxyModel->mapToSource(materialProxyModel->index(row, 0)).row());
    int id = record.value("id").toInt();

    QSqlQuery query;
    query.prepare("SELECT drawing FROM Material WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        QByteArray data = query.value(0).toByteArray();
        if (data.isEmpty()) {
            QMessageBox::information(this, "提示", "该物料没有图纸。");
            return;
        }
        displayDataInPreview(data);
    } else {
        QMessageBox::warning(this, "错误", "无法获取图纸数据：" + query.lastError().text());
    }
}

// 查看实物照片槽函数
void MainWindow::on_viewPhotoButton_clicked()
{
    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个物料。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = materialModel->record(materialProxyModel->mapToSource(materialProxyModel->index(row, 0)).row());
    int id = record.value("id").toInt();

    QSqlQuery query;
    query.prepare("SELECT photo FROM Material WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        QByteArray data = query.value(0).toByteArray();
        if (data.isEmpty()) {
            QMessageBox::information(this, "提示", "该物料没有照片。");
            return;
        }
        displayDataInPreview(data);
    } else {
        QMessageBox::warning(this, "错误", "无法获取照片数据：" + query.lastError().text());
    }
}

// 显示预览数据的辅助函数
void MainWindow::displayDataInPreview(const QByteArray &data)
{
    QPixmap pixmap;
    if (pixmap.loadFromData(data)) {
        ui->previewLabel->setPixmap(pixmap.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        QMessageBox::warning(this, "警告", "无法显示数据。");
    }
}

// 添加不良品槽函数
void MainWindow::on_addDefectiveButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个物料。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = materialModel->record(materialProxyModel->mapToSource(materialProxyModel->index(row, 0)).row());

    MaterialDialog dialog(currentUser, this);
    dialog.setMaterialData(record);
    dialog.enableDefectiveQuantityEditing(true);
    if (dialog.exec() == QDialog::Accepted) {
        defectiveModel->select();
        materialModel->select(); // 更新物料表
    }
}

// 编辑不良品槽函数
void MainWindow::on_editDefectiveButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->defectiveTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的不良品。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = defectiveModel->record(row);

    MaterialDialog dialog(currentUser, this);
    dialog.setMaterialData(record);
    dialog.enableDefectiveQuantityEditing(true);
    if (dialog.exec() == QDialog::Accepted) {
        defectiveModel->select();
        materialModel->select(); // 更新物料表
    }
}

// 删除不良品槽函数
void MainWindow::on_deleteDefectiveButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->defectiveTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的不良品。");
        return;
    }

    int ret = QMessageBox::question(this, "确认删除", "确定要将该物料的不良品数量清零吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        foreach (const QModelIndex &index, selectedRows) {
            int row = index.row();
            QSqlRecord record = defectiveModel->record(row);

            // 将 defective_quantity 设为 0
            QSqlQuery query;
            query.prepare("UPDATE Material SET defective_quantity = 0 WHERE id = :id");
            query.bindValue(":id", record.value("id"));
            if (!query.exec()) {
                QMessageBox::critical(this, "错误", "无法删除不良品：" + query.lastError().text());
            } else {
                defectiveModel->select();
                materialModel->select(); // 更新物料表
            }
        }
    }
}

// 添加 BOM 槽函数
void MainWindow::on_addBOMButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    BOMDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        bomModel->select();
    }
}

// 编辑 BOM 槽函数
void MainWindow::on_editBOMButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->bomTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的 BOM。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = bomModel->record(row);

    BOMDialog dialog(this);
    dialog.setBOMData(record);
    if (dialog.exec() == QDialog::Accepted) {
        bomModel->select();
    }
}

// 删除 BOM 槽函数
void MainWindow::on_deleteBOMButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->bomTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的 BOM。");
        return;
    }

    int ret = QMessageBox::question(this, "确认删除", "确定要删除选中的 BOM 及其物料清单吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        foreach (const QModelIndex &index, selectedRows) {
            int row = index.row();
            int bomId = bomModel->record(row).value("id").toInt();

            QSqlDatabase db = QSqlDatabase::database();
            db.transaction();

            // 删除相关 BOM_Material 记录
            QSqlQuery deleteBOMMaterials;
            deleteBOMMaterials.prepare("DELETE FROM BOM_Material WHERE bom_id = :bom_id");
            deleteBOMMaterials.bindValue(":bom_id", bomId);
            if (!deleteBOMMaterials.exec()) {
                db.rollback();
                QMessageBox::critical(this, "错误", "无法删除 BOM 物料清单：" + deleteBOMMaterials.lastError().text());
                return;
            }

            // 删除 BOM
            bomModel->removeRow(row);
            if (!bomModel->submitAll()) {
                db.rollback();
                QMessageBox::critical(this, "错误", "无法删除 BOM：" + bomModel->lastError().text());
                return;
            }

            db.commit();
            QMessageBox::information(this, "成功", "BOM及其物料清单已成功删除。");
            bomModel->select();
        }
    }
}

// 添加订单槽函数
void MainWindow::on_addOrderButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    OrderDialog dialog(currentUser, this);
    if (dialog.exec() == QDialog::Accepted) {
        // 刷新所有订单模型
        for (auto model : orderModels) {
            model->select();
        }
    }
}

// 编辑订单槽函数
void MainWindow::on_editOrderButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    int currentIndex = ui->orderTabWidget->currentIndex();
    QString currentStatus = ui->orderTabWidget->tabText(currentIndex);

    QTableView *currentTableView = orderTableViews[currentStatus];
    QModelIndexList selectedRows = currentTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的订单。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = orderModels[currentStatus]->record(row);

    OrderDialog dialog(currentUser, this);
    dialog.setOrderData(record);
    if (dialog.exec() == QDialog::Accepted) {
        // 刷新所有订单模型
        for (auto model : orderModels) {
            model->select();
        }
    }
}

// 删除订单槽函数
void MainWindow::on_deleteOrderButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    int currentIndex = ui->orderTabWidget->currentIndex();
    QString currentStatus = ui->orderTabWidget->tabText(currentIndex);

    QTableView *currentTableView = orderTableViews[currentStatus];
    QModelIndexList selectedRows = currentTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的订单。");
        return;
    }

    int ret = QMessageBox::question(this, "确认删除", "确定要删除选中的订单吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        foreach (const QModelIndex &index, selectedRows) {
            int row = index.row();
            orderModels[currentStatus]->removeRow(row);
            if (!orderModels[currentStatus]->submitAll()) {
                QMessageBox::critical(this, "错误", "无法删除订单：" + orderModels[currentStatus]->lastError().text());
            }
        }
        // 刷新所有订单模型
        for (auto model : orderModels) {
            model->select();
        }
    }
}

// 撤销槽函数
void MainWindow::on_undoButton_clicked()
{
    if (undoStack->canUndo()) {
        undoStack->undo();
    }
}

// 重做槽函数
void MainWindow::on_redoButton_clicked()
{
    if (undoStack->canRedo()) {
        undoStack->redo();
    }
}

// 添加供应商槽函数
void MainWindow::on_addSupplierButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    SupplierDialog dialog(currentUser, this); // 传递 currentUser
    if (dialog.exec() == QDialog::Accepted) {
        supplierModel->select();
    }
}

// 编辑供应商槽函数
void MainWindow::on_editSupplierButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->supplierTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的供应商。");
        return;
    }

    int row = selectedRows.first().row();
    QSqlRecord record = supplierModel->record(supplierProxyModel->mapToSource(supplierProxyModel->index(row, 0)).row());

    SupplierDialog dialog(currentUser, this); // 传递 currentUser
    dialog.setSupplierData(record);
    if (dialog.exec() == QDialog::Accepted) {
        supplierModel->select();
        materialModel->select(); // 更新物料表以反映供应商名称的变化
    }
}

// 删除供应商槽函数
void MainWindow::on_deleteSupplierButton_clicked()
{
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先登录。");
        return;
    }

    QModelIndexList selectedRows = ui->supplierTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的供应商。");
        return;
    }

    int ret = QMessageBox::question(this, "确认删除", "确定要删除选中的供应商及其所有物料吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        foreach (const QModelIndex &index, selectedRows) {
            int row = index.row();
            QString supplierId = supplierProxyModel->data(supplierProxyModel->index(row, supplierModel->fieldIndex("supplier_id"))).toString();

            QSqlDatabase db = QSqlDatabase::database();
            db.transaction();

            // 删除相关物料
            QSqlQuery deleteMaterials;
            deleteMaterials.prepare("DELETE FROM Material WHERE supplier_id = :supplier_id");
            deleteMaterials.bindValue(":supplier_id", supplierId);
            if (!deleteMaterials.exec()) {
                db.rollback();
                QMessageBox::critical(this, "错误", "无法删除供应商的物料：" + deleteMaterials.lastError().text());
                return;
            }

            // 删除供应商
            QModelIndex proxyIndex = supplierProxyModel->index(row, 0);
            QModelIndex sourceIndex = supplierProxyModel->mapToSource(proxyIndex);
            if (!supplierModel->removeRow(sourceIndex.row())) {
                db.rollback();
                QMessageBox::critical(this, "错误", "无法删除供应商。");
                return;
            }

            if (!supplierModel->submitAll()) { // 在 QSqlTableModel 上调用 submitAll()
                db.rollback();
                QMessageBox::critical(this, "错误", "无法删除供应商：" + supplierModel->lastError().text());
                return;
            }

            db.commit();
            QMessageBox::information(this, "成功", "供应商及其物料已成功删除。");
            supplierModel->select();
            materialModel->select();
        }
    }
}

// 供应商搜索槽函数
void MainWindow::on_supplierSearchLineEdit_textChanged(const QString &text)
{
    QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
    supplierProxyModel->setFilterRegularExpression(regExp);
}

// 物料搜索槽函数
void MainWindow::on_materialSearchLineEdit_textChanged(const QString &text)
{
    QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
    materialProxyModel->setFilterRegularExpression(regExp);
}

// 不良品搜索槽函数
void MainWindow::on_defectiveSearchLineEdit_textChanged(const QString &text)
{
    QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
    defectiveProxyModel->setFilterRegularExpression(regExp);
}

// BOM搜索槽函数
void MainWindow::on_bomSearchLineEdit_textChanged(const QString &text)
{
    QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
    bomProxyModel->setFilterRegularExpression(regExp);
}

// 物料表选择变化槽函数
void MainWindow::onMaterialTableViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected); // 避免未使用参数的警告

    QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        // 只显示第一个选中物料的照片
        int row = selectedRows.first().row();
        QSqlRecord record = materialModel->record(materialProxyModel->mapToSource(materialProxyModel->index(row, 0)).row());
        int id = record.value("id").toInt();

        // 加载实物照片进行预览
        QSqlQuery query;
        query.prepare("SELECT photo FROM Material WHERE id = :id");
        query.bindValue(":id", id);
        if (query.exec() && query.next()) {
            QByteArray data = query.value(0).toByteArray();
            if (!data.isEmpty()) {
                displayDataInPreview(data);
            } else {
                ui->previewLabel->clear();
                ui->previewLabel->setText("无预览图像");
            }
        }
    } else {
        ui->previewLabel->clear();
        ui->previewLabel->setText("无预览图像");
    }
}

// 供应商选择变化槽函数
void MainWindow::onSupplierSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected); // 避免未使用参数的警告

    QModelIndexList selectedRows = ui->supplierTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        // 只处理第一个选中的供应商
        int row = selectedRows.first().row();
        QString supplierId = supplierProxyModel->data(supplierProxyModel->index(row, supplierModel->fieldIndex("supplier_id"))).toString();

        // 获取 'supplier_id' 列的索引
        int supplierIdColumn = materialModel->fieldIndex("supplier_id");
        if (supplierIdColumn != -1) {
            materialProxyModel->setFilterKeyColumn(supplierIdColumn);
            materialProxyModel->setFilterFixedString(supplierId);
        }

        // 设置模型到 supplierMaterialTableView
        ui->supplierMaterialTableView->setModel(materialProxyModel);
        ui->supplierMaterialTableView->setItemDelegate(new AlignmentDelegate(this));
        ui->supplierMaterialTableView->setFont(QFont("Microsoft YaHei", 10));
        ui->supplierMaterialTableView->verticalHeader()->setDefaultSectionSize(24);
        ui->supplierMaterialTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->supplierMaterialTableView->horizontalHeader()->setStretchLastSection(true);
        ui->supplierMaterialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->supplierMaterialTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        ui->supplierMaterialTableView->setSortingEnabled(true);

        // 检查是否有物料
        if (materialProxyModel->rowCount() == 0) {
            QMessageBox::information(this, "提示", "该供应商暂无物料。");
        }
    } else {
        // 如果没有选择供应商，清除过滤
        materialProxyModel->setFilterFixedString("");
        materialProxyModel->setFilterKeyColumn(-1); // 显示所有列
        ui->supplierMaterialTableView->setModel(nullptr);
    }
}

// BOM选择变化槽函数
void MainWindow::onBOMSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected); // 避免未使用参数的警告

    QModelIndexList selectedRows = ui->bomTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        int row = selectedRows.first().row();
        int bomId = bomModel->record(row).value("id").toInt();

        // 设置一个新的模型来显示 BOM 中的所有物料
        QSqlQueryModel *bomMaterialModel = new QSqlQueryModel(this);
        QString queryStr = QString("SELECT m.material_number, m.description, bm.quantity, m.unit_price, (bm.quantity * m.unit_price) AS total_price "
                                   "FROM BOM_Material bm "
                                   "JOIN Material m ON bm.material_id = m.id "
                                   "WHERE bm.bom_id = %1").arg(bomId);
        bomMaterialModel->setQuery(queryStr);
        if (bomMaterialModel->lastError().isValid()) {
            QMessageBox::critical(this, "错误", "无法加载 BOM 物料：" + bomMaterialModel->lastError().text());
            return;
        }

        // 设置表头
        bomMaterialModel->setHeaderData(0, Qt::Horizontal, "物料号");
        bomMaterialModel->setHeaderData(1, Qt::Horizontal, "描述");
        bomMaterialModel->setHeaderData(2, Qt::Horizontal, "数量");
        bomMaterialModel->setHeaderData(3, Qt::Horizontal, "单价");
        bomMaterialModel->setHeaderData(4, Qt::Horizontal, "总价");

        // 设置模型到 bomMaterialTableView
        ui->bomMaterialTableView->setModel(bomMaterialModel);
        ui->bomMaterialTableView->setItemDelegate(new AlignmentDelegate(this));
        ui->bomMaterialTableView->setFont(QFont("Microsoft YaHei", 10));
        ui->bomMaterialTableView->verticalHeader()->setDefaultSectionSize(24);
        ui->bomMaterialTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->bomMaterialTableView->horizontalHeader()->setStretchLastSection(true);
        ui->bomMaterialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->bomMaterialTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        ui->bomMaterialTableView->setSortingEnabled(true);
    } else {
        ui->bomMaterialTableView->setModel(nullptr);
    }
}

// 订单状态变化槽函数
void MainWindow::on_orderStatusChanged(int orderId, const QString &newStatus)
{
    // 现有实现保持不变
    QSqlQuery query;
    query.prepare("SELECT status FROM `Order` WHERE id = :id"); // 确认表名为 Order
    query.bindValue(":id", orderId);
    if (query.exec() && query.next()) {
        QString oldStatus = query.value("status").toString();

        // 更新订单状态
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE `Order` SET status = :status, updated_at = NOW() WHERE id = :id"); // 确认表名为 Order
        updateQuery.bindValue(":status", newStatus);
        updateQuery.bindValue(":id", orderId);
        if (!updateQuery.exec()) {
            QMessageBox::warning(this, "警告", "无法更新订单状态：" + updateQuery.lastError().text());
            return;
        }

        // 根据状态变化处理库存
        if (oldStatus == "预下单" && newStatus == "制作中") {
            // 扣减物料库存
            processMaterialInventory(orderId, "deduct");
        } else if (oldStatus == "制作中" && newStatus == "制作完毕") {
            // 增加成品库存
            processProductInventory(orderId, "increase");
        } else if (oldStatus == "制作完毕" && newStatus == "已出库") {
            // 扣减成品库存
            processProductInventory(orderId, "deduct");
        }

        // 添加订单操作日志
        QSqlQuery logQuery;
        logQuery.prepare("INSERT INTO OrderLog (order_id, operation_type, operator, operation_date, details) "
                         "VALUES (:order_id, 'UPDATE', :operator, NOW(), :details)");
        logQuery.bindValue(":order_id", orderId);
        logQuery.bindValue(":operator", currentUser);
        logQuery.bindValue(":details", QString("订单状态从 '%1' 变为 '%2'").arg(oldStatus).arg(newStatus));
        if (!logQuery.exec()) {
            QMessageBox::warning(this, "警告", "无法记录订单日志：" + logQuery.lastError().text());
        }
    }
}

// 处理物料库存的辅助函数
void MainWindow::processMaterialInventory(int orderId, const QString &operation)
{
    // 现有实现保持不变
    QSqlQuery productQuery;
    productQuery.prepare("SELECT product_id, quantity FROM Order_Product WHERE order_id = :order_id");
    productQuery.bindValue(":order_id", orderId);
    if (productQuery.exec()) {
        while (productQuery.next()) {
            int productId = productQuery.value("product_id").toInt();
            int productQuantity = productQuery.value("quantity").toInt();

            // 查询成品对应的 BOM
            QSqlQuery bomQuery;
            bomQuery.prepare("SELECT material_id, quantity FROM BOM_Material WHERE bom_id = :bom_id");
            bomQuery.bindValue(":bom_id", productId);
            if (bomQuery.exec()) {
                while (bomQuery.next()) {
                    int materialId = bomQuery.value("material_id").toInt();
                    int materialQuantityRequired = bomQuery.value("quantity").toInt() * productQuantity;

                    // 更新物料库存
                    QSqlQuery materialQuery;
                    QString sql = (operation == "deduct") ? "UPDATE Material SET good_quantity = good_quantity - :quantity WHERE id = :id"
                                                          : "UPDATE Material SET good_quantity = good_quantity + :quantity WHERE id = :id";
                    materialQuery.prepare(sql);
                    materialQuery.bindValue(":quantity", materialQuantityRequired);
                    materialQuery.bindValue(":id", materialId);
                    if (!materialQuery.exec()) {
                        QMessageBox::warning(this, "警告", "无法更新物料库存：" + materialQuery.lastError().text());
                    } else {
                        // 添加物料操作日志
                        QSqlQuery logQuery;
                        logQuery.prepare("INSERT INTO MaterialLog (material_id, operation_type, operator, operation_date, details) "
                                         "VALUES (:material_id, 'UPDATE', :operator, NOW(), :details)");
                        logQuery.bindValue(":material_id", materialId);
                        logQuery.bindValue(":operator", currentUser);
                        QString detail = (operation == "deduct") ? QString("物料扣减 %1").arg(materialQuantityRequired)
                                                                 : QString("物料增加 %1").arg(materialQuantityRequired);
                        logQuery.bindValue(":details", detail);
                        if (!logQuery.exec()) {
                            QMessageBox::warning(this, "警告", "无法记录物料日志：" + logQuery.lastError().text());
                        }
                    }
                }
            }
        }
    }
}

// 处理成品库存的辅助函数
void MainWindow::processProductInventory(int orderId, const QString &operation)
{
    // 现有实现保持不变
    QSqlQuery productQuery;
    productQuery.prepare("SELECT product_id, quantity FROM Order_Product WHERE order_id = :order_id");
    productQuery.bindValue(":order_id", orderId);
    if (productQuery.exec()) {
        while (productQuery.next()) {
            int productId = productQuery.value("product_id").toInt();
            int productQuantity = productQuery.value("quantity").toInt();

            // 更新成品库存（假设成品也在 Material 表中）
            QSqlQuery productUpdateQuery;
            QString sql = (operation == "deduct") ? "UPDATE Material SET good_quantity = good_quantity - :quantity WHERE id = :id"
                                                  : "UPDATE Material SET good_quantity = good_quantity + :quantity WHERE id = :id";
            productUpdateQuery.prepare(sql);
            productUpdateQuery.bindValue(":quantity", productQuantity);
            productUpdateQuery.bindValue(":id", productId);
            if (!productUpdateQuery.exec()) {
                QMessageBox::warning(this, "警告", "无法更新成品库存：" + productUpdateQuery.lastError().text());
            } else {
                // 添加物料操作日志
                QSqlQuery logQuery;
                logQuery.prepare("INSERT INTO MaterialLog (material_id, operation_type, operator, operation_date, details) "
                                 "VALUES (:material_id, 'UPDATE', :operator, NOW(), :details)");
                logQuery.bindValue(":material_id", productId);
                logQuery.bindValue(":operator", currentUser);
                QString detail = (operation == "deduct") ? QString("成品库存扣减 %1").arg(productQuantity)
                                                         : QString("成品库存增加 %1").arg(productQuantity);
                logQuery.bindValue(":details", detail);
                if (!logQuery.exec()) {
                    QMessageBox::warning(this, "警告", "无法记录物料日志：" + logQuery.lastError().text());
                }
            }
        }
    }
}

// 供应商搜索槽函数
//void MainWindow::on_supplierSearchLineEdit_textChanged(const QString &text)
// {
//     QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
//     supplierProxyModel->setFilterRegularExpression(regExp);
// }

// 物料搜索槽函数
// void MainWindow::on_materialSearchLineEdit_textChanged(const QString &text)
// {
//     QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
//     materialProxyModel->setFilterRegularExpression(regExp);
// }

// // 不良品搜索槽函数
// void MainWindow::on_defectiveSearchLineEdit_textChanged(const QString &text)
// {
//     QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
//     defectiveProxyModel->setFilterRegularExpression(regExp);
// }

// // BOM搜索槽函数
// void MainWindow::on_bomSearchLineEdit_textChanged(const QString &text)
// {
//     QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
//     bomProxyModel->setFilterRegularExpression(regExp);
// }

// // 物料表选择变化槽函数
// void MainWindow::onMaterialTableViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
// {
//     Q_UNUSED(deselected); // 避免未使用参数的警告

//     QModelIndexList selectedRows = ui->materialTableView->selectionModel()->selectedRows();
//     if (!selectedRows.isEmpty()) {
//         // 只显示第一个选中物料的照片
//         int row = selectedRows.first().row();
//         QSqlRecord record = materialModel->record(materialProxyModel->mapToSource(materialProxyModel->index(row, 0)).row());
//         int id = record.value("id").toInt();

//         // 加载实物照片进行预览
//         QSqlQuery query;
//         query.prepare("SELECT photo FROM Material WHERE id = :id");
//         query.bindValue(":id", id);
//         if (query.exec() && query.next()) {
//             QByteArray data = query.value(0).toByteArray();
//             if (!data.isEmpty()) {
//                 displayDataInPreview(data);
//             } else {
//                 ui->previewLabel->clear();
//                 ui->previewLabel->setText("无预览图像");
//             }
//         }
//     } else {
//         ui->previewLabel->clear();
//         ui->previewLabel->setText("无预览图像");
//     }
// }

// 供应商选择变化槽函数
//void MainWindow::onSupplierSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
// {
//     Q_UNUSED(deselected); // 避免未使用参数的警告

//     QModelIndexList selectedRows = ui->supplierTableView->selectionModel()->selectedRows();
//     if (!selectedRows.isEmpty()) {
//         // 只处理第一个选中的供应商
//         int row = selectedRows.first().row();
//         QString supplierId = supplierProxyModel->data(supplierProxyModel->index(row, supplierModel->fieldIndex("supplier_id"))).toString();

//         // 使用代理模型设置过滤条件
//         materialProxyModel->setFilter(QString("supplier_id = '%1'").arg(supplierId));

//         // 设置模型到 supplierMaterialTableView
//         ui->supplierMaterialTableView->setModel(materialProxyModel);
//         ui->supplierMaterialTableView->setItemDelegate(new AlignmentDelegate(this));
//         ui->supplierMaterialTableView->setFont(QFont("Microsoft YaHei", 10));
//         ui->supplierMaterialTableView->verticalHeader()->setDefaultSectionSize(24);
//         ui->supplierMaterialTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//         ui->supplierMaterialTableView->horizontalHeader()->setStretchLastSection(true);
//         ui->supplierMaterialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//         ui->supplierMaterialTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
//         ui->supplierMaterialTableView->setSortingEnabled(true);

//         // 检查是否有物料
//         if (materialProxyModel->rowCount() == 0) {
//             QMessageBox::information(this, "提示", "该供应商暂无物料。");
//         }
//     } else {
//         // 如果没有选择供应商，清除过滤
//         materialProxyModel->setFilter("");
//         ui->supplierMaterialTableView->setModel(nullptr);
//     }
// }

// BOM选择变化槽函数
//void MainWindow::onBOMSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
// {
//     Q_UNUSED(deselected); // 避免未使用参数的警告

//     QModelIndexList selectedRows = ui->bomTableView->selectionModel()->selectedRows();
//     if (!selectedRows.isEmpty()) {
//         int row = selectedRows.first().row();
//         int bomId = bomModel->record(row).value("id").toInt();

//         // 设置一个新的模型来显示 BOM 中的所有物料
//         QSqlQueryModel *bomMaterialModel = new QSqlQueryModel(this);
//         QString queryStr = QString("SELECT m.material_number, m.description, bm.quantity, m.unit_price, (bm.quantity * m.unit_price) AS total_price "
//                                    "FROM BOM_Material bm "
//                                    "JOIN Material m ON bm.material_id = m.id "
//                                    "WHERE bm.bom_id = %1").arg(bomId);
//         bomMaterialModel->setQuery(queryStr);
//         if (bomMaterialModel->lastError().isValid()) {
//             QMessageBox::critical(this, "错误", "无法加载 BOM 物料：" + bomMaterialModel->lastError().text());
//             delete bomMaterialModel;
//             return;
//         }

//         // 设置表头
//         bomMaterialModel->setHeaderData(0, Qt::Horizontal, "物料号");
//         bomMaterialModel->setHeaderData(1, Qt::Horizontal, "描述");
//         bomMaterialModel->setHeaderData(2, Qt::Horizontal, "数量");
//         bomMaterialModel->setHeaderData(3, Qt::Horizontal, "单价");
//         bomMaterialModel->setHeaderData(4, Qt::Horizontal, "总价");

//         // 设置模型到 bomMaterialTableView
//         ui->bomMaterialTableView->setModel(bomMaterialModel);
//         ui->bomMaterialTableView->setItemDelegate(new AlignmentDelegate(this));
//         ui->bomMaterialTableView->setFont(QFont("Microsoft YaHei", 10));
//         ui->bomMaterialTableView->verticalHeader()->setDefaultSectionSize(24);
//         ui->bomMaterialTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//         ui->bomMaterialTableView->horizontalHeader()->setStretchLastSection(true);
//         ui->bomMaterialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//         ui->bomMaterialTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
//         ui->bomMaterialTableView->setSortingEnabled(true);
//     } else {
//         ui->bomMaterialTableView->setModel(nullptr);
//     }
// }

// 用户操作相关槽函数的实现保持不变...

// 撤销和重做相关槽函数
// void MainWindow::on_undoButton_clicked()
// {
//     if (undoStack->canUndo()) {
//         undoStack->undo();
//     }
// }

// void MainWindow::on_redoButton_clicked()
// {
//     if (undoStack->canRedo()) {
//         undoStack->redo();
//     }
// }

// 供应商管理操作槽函数的实现保持不变...

// 搜索槽函数的实现保持不变...

// 其他辅助函数的实现保持不变...

