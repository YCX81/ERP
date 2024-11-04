import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: materialDialog
    width: 500
    height: 750
    visible: true
    title: "物料信息"

    ScrollView {
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 10
            padding: 20

            GridLayout {
                id: gridLayout
                columns: 3
                rowSpacing: 10
                columnSpacing: 10
                Layout.fillWidth: true

                // 类别
                Label {
                    text: "类别："
                    Layout.row: 0
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                ComboBox {
                    id: categoryComboBox
                    Layout.row: 0
                    Layout.column: 2
                    Layout.fillWidth: true
                    model: backend.categoryList
                    textRole: "name"
                    delegate: ItemDelegate {
                        text: modelData.name + " - " + modelData.letterCode
                    }
                }

                // 描述
                Label {
                    text: "描述："
                    Layout.row: 1
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                TextArea {
                    id: descriptionTextEdit
                    Layout.row: 1
                    Layout.column: 2
                    Layout.columnSpan: 1
                    height: 60
                }

                // 数量
                Label {
                    text: "数量："
                    Layout.row: 2
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                SpinBox {
                    id: quantitySpinBox
                    from: 0
                    to: 1000000
                    Layout.row: 2
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 单价（不含税）
                Label {
                    text: "单价（不含税）："
                    Layout.row: 3
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                DoubleSpinBox {
                    id: unitPriceDoubleSpinBox
                    from: 0.00
                    to: 9999999.99
                    decimals: 2
                    Layout.row: 3
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 来源
                Label {
                    text: "来源："
                    Layout.row: 4
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                ComboBox {
                    id: sourceComboBox
                    Layout.row: 4
                    Layout.column: 2
                    Layout.fillWidth: true
                    model: ["自制 (P)", "采购 (M)"]
                }

                // 版本号
                Label {
                    text: "版本号："
                    Layout.row: 5
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                SpinBox {
                    id: versionSpinBox
                    from: 0
                    to: 1000
                    Layout.row: 5
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 序列号
                Label {
                    text: "序列号："
                    Layout.row: 6
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                TextField {
                    id: serialNumberLineEdit
                    Layout.row: 6
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 库位号
                Label {
                    text: "库位号："
                    Layout.row: 7
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                TextField {
                    id: locationLineEdit
                    Layout.row: 7
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 良品数量
                Label {
                    text: "良品数量："
                    Layout.row: 8
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                SpinBox {
                    id: goodQuantitySpinBox
                    from: 0
                    to: 1000000
                    Layout.row: 8
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 不良品数量
                Label {
                    text: "不良品数量："
                    Layout.row: 9
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                SpinBox {
                    id: defectiveQuantitySpinBox
                    from: 0
                    to: 1000000
                    Layout.row: 9
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 供应商
                Label {
                    text: "供应商："
                    Layout.row: 10
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                ComboBox {
                    id: supplierComboBox
                    Layout.row: 10
                    Layout.column: 2
                    Layout.fillWidth: true
                    model: backend.supplierList
                    delegate: ItemDelegate {
                        text: modelData
                    }
                }

                // 供应商料号
                Label {
                    text: "供应商料号："
                    Layout.row: 11
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                TextField {
                    id: supplierMaterialNumberLineEdit
                    Layout.row: 11
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 货期（天）
                Label {
                    text: "货期（天）："
                    Layout.row: 12
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                SpinBox {
                    id: deliverySpinBox
                    from: 0
                    to: 365
                    Layout.row: 12
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 更新日期
                Label {
                    text: "更新日期："
                    Layout.row: 23
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                DateTimeEdit {
                    id: updateDateTimeEdit
                    readOnly: true
                    displayFormat: "yyyy-MM-dd HH:mm:ss"
                    calendarPopup: true
                    Layout.row: 23
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 物料维护人
                Label {
                    text: "物料维护人："
                    Layout.row: 22
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                TextField {
                    id: maintainerLineEdit
                    readOnly: true
                    text: backend.currentMaintainer
                    Layout.row: 22
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 备注
                Label {
                    text: "备注："
                    Layout.row: 24
                    Layout.column: 0
                    verticalAlignment: Label.AlignTop
                }
                TextArea {
                    id: noteTextEdit
                    Layout.row: 24
                    Layout.column: 2
                    Layout.columnSpan: 1
                    height: 60
                }

                // 物料号
                Label {
                    text: "物料号"
                    Layout.row: 14
                    Layout.column: 0
                    verticalAlignment: Label.AlignVCenter
                }
                TextField {
                    id: materialNumberLineEdit
                    Layout.row: 14
                    Layout.column: 2
                    Layout.fillWidth: true
                }

                // 上传实物照片布局
                RowLayout {
                    id: photoLayout
                    Layout.row: 21
                    Layout.column: 0
                    Layout.columnSpan: 3
                    spacing: 10

                    Button {
                        id: uploadPhotoButton
                        text: "上传实物照片"
                        onClicked: {
                            fileDialog.title = "选择实物照片"
                            fileDialog.filters = "图片文件 (*.png *.jpg *.jpeg)"
                            fileDialog.selectMultiple = false
                            fileDialog.open()
                            fileDialog.onAccepted: {
                                var filePath = fileDialog.fileUrl
                                backend.uploadPhoto(filePath)
                                photoStatusLabel.text = "照片已上传"
                            }
                        }
                    }

                    Label {
                        id: photoStatusLabel
                        text: "未上传"
                        verticalAlignment: Label.AlignVCenter
                    }

                    Button {
                        id: downloadPhotoButton
                        text: "下载实物照片"
                        onClicked: {
                            fileDialog.title = "下载实物照片"
                            fileDialog.filters = "图片文件 (*.png *.jpg *.jpeg)"
                            fileDialog.selectMultiple = false
                            fileDialog.open()
                            fileDialog.onAccepted: {
                                var filePath = fileDialog.fileUrl
                                backend.downloadPhoto(filePath)
                                // 显示下载成功的消息
                            }
                        }
                    }
                }

                // 上传图纸布局
                RowLayout {
                    id: fileLayout
                    Layout.row: 20
                    Layout.column: 0
                    Layout.columnSpan: 3
                    spacing: 10

                    Button {
                        id: uploadDrawingButton
                        text: "上传图纸"
                        onClicked: {
                            fileDialog.title = "选择图纸"
                            fileDialog.filters = "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)"
                            fileDialog.selectMultiple = false
                            fileDialog.open()
                            fileDialog.onAccepted: {
                                var filePath = fileDialog.fileUrl
                                backend.uploadDrawing(filePath)
                                drawingStatusLabel.text = "图纸已上传"
                            }
                        }
                    }

                    Label {
                        id: drawingStatusLabel
                        text: "未上传"
                        verticalAlignment: Label.AlignVCenter
                    }

                    Button {
                        id: downloadDrawingButton
                        text: "下载图纸"
                        onClicked: {
                            fileDialog.title = "下载图纸"
                            fileDialog.filters = "PDF 文件 (*.pdf);;图片文件 (*.png *.jpg *.jpeg)"
                            fileDialog.selectMultiple = false
                            fileDialog.open()
                            fileDialog.onAccepted: {
                                var filePath = fileDialog.fileUrl
                                backend.downloadDrawing(filePath)
                                // 显示下载成功的消息
                            }
                        }
                    }
                }

                // 生成物料号按钮
                Button {
                    id: materialNumberGenerateButton
                    text: "生成物料号"
                    Layout.row: 15
                    Layout.column: 2
                    Layout.alignment: Qt.AlignRight
                    onClicked: {
                        var category = categoryComboBox.currentData
                        var supplier = supplierComboBox.currentText.split(" - ")[0]
                        var version = versionSpinBox.value
                        if (category) {
                            var materialNumber = backend.generateMaterialNumber(category.letterCode, supplier, version)
                            materialNumberLineEdit.text = materialNumber
                        } else {
                            // 显示警告提示用户选择类别
                        }
                    }
                }
            }

            // 保存按钮
            Button {
                id: saveButton
                text: "保存"
                Layout.alignment: Qt.AlignRight
                width: 100
                onClicked: {
                    var record = {
                        material_number: materialNumberLineEdit.text,
                        description: descriptionTextEdit.text,
                        quantity: quantitySpinBox.value,
                        unit_price: unitPriceDoubleSpinBox.value,
                        self_made_or_purchase: sourceComboBox.currentIndex === 0 ? "P" : "M",
                        version_number: versionSpinBox.value,
                        serial_number: serialNumberLineEdit.text,
                        location_number: locationLineEdit.text,
                        good_quantity: goodQuantitySpinBox.value,
                        defective_quantity: defectiveQuantitySpinBox.value,
                        supplier: supplierComboBox.currentText.split(" - ")[0],
                        supplier_material_number: supplierMaterialNumberLineEdit.text,
                        delivery_period: deliverySpinBox.value,
                        material_maintainer: maintainerLineEdit.text,
                        remarks: noteTextEdit.text,
                        update_date: new Date().toISOString()
                        // 添加id、drawing和photo等字段根据需要
                    }

                    var errorMessage = ""
                    var isValid = backend.validateInput(record, errorMessage)
                    if (!isValid) {
                        // 显示错误消息
                        console.log("验证失败:", errorMessage)
                        return
                    }

                    var success = backend.saveMaterialData(record)
                    if (success) {
                        // 显示保存成功的消息
                        console.log("保存成功")
                    } else {
                        // 显示保存失败的消息
                        console.log("保存失败")
                    }
                }
            }

            // 文件对话框
            FileDialog {
                id: fileDialog
                visible: false
            }
        }
    }
}
