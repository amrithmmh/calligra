/* This file is part of the KDE project
 * Copyright (C) 2014 Dan Leinir Turthra Jensen <admin@leinir.dk>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

import QtQuick 2.11
import org.kde.kirigami 2.7 as Kirigami
import org.calligra 1.0
import "../components"

Kirigami.ScrollablePage {
    id: base;
    objectName: "WelcomePageWords"
    title: "Create a Text Document"
    actions {
        main: Kirigami.Action {
            text: "Create Custom Document..."
            icon.name: "configure"
            onTriggered: {
                pageStack.push(customDocWords);
            }
        }
    }
    Component { id: customDocWords; CustomDocWords { } }
    function activateTemplate(templateFile) {
        console.debug(templateFile);
        if(templateFile.slice(-1) === "/" || templateFile === "") {
            return;
        }
        openFile("template://" + templateFile);
    }
    TemplatesModel {
        id: wordsTemplates;
        templateType: WORDS_MIME_TYPE;
    }
    Flickable {
        id: wordsFlickable;
        anchors {
            fill: parent;
            margins: Kirigami.Units.largeSpacing;
            bottomMargin: 0;
        }
        contentWidth: templatesFlow.width;
        contentHeight: templatesFlow.height;
        clip: true;
        Flow {
            id: templatesFlow;
            width: base.width - Kirigami.Units.largeSpacing * 2;
            spacing: Kirigami.Units.largeSpacing * 3;
            Repeater {
                id: wordsTemplatesRepeater;
                model: wordsTemplates;
                Column {
                    width: (templatesFlow.width / 5) - Kirigami.Units.largeSpacing * 3;
                    height: width * 1.4;
                    spacing: Kirigami.Units.largeSpacing;
                    Item {
                        width: parent.width;
                        height: parent.height - templateName.height - Kirigami.Units.largeSpacing;
                        Image {
                            anchors.centerIn: parent;
                            height: parent.height - Kirigami.Units.largeSpacing * 2;
                            width: height;
                            source: model.thumbnail ? model.thumbnail : "";
                            fillMode: Image.PreserveAspectFit;
                            smooth: true;
                        }
                        Rectangle {
                            anchors.fill: parent;
                            color: model.thumbnail ? "transparent" : model.color;
                            border {
                                width: 1;
                                color: "#c1cdd1";
                            }
                            opacity: 0.6;
                        }
                        Row {
                            anchors {
                                bottom: parent.bottom;
                                right: parent.right;
                                margins: Kirigami.Units.largeSpacing;
                            }
                            height: Kirigami.Units.largeSpacing * 2;
                            spacing: Kirigami.Units.largeSpacing;
                            property QtObject colorModel: model.variants;
                            Repeater {
                                model: parent.colorModel;
                                Rectangle {
                                    height: Kirigami.Units.largeSpacing * 2;
                                    width: height;
                                    radius: Kirigami.Units.largeSpacing;
                                    color: model.color;
                                }
                            }
                        }
                        MouseArea {
                            anchors.fill: parent;
                            onClicked: {
                                if(model.variantCount === 0) {
                                    activateTemplate(model.url);
                                }
                                else {
                                    // then there are variants to choose between, let the user see!
                                    wordsVariantSelector.model = model.variants;
                                    wordsVariantSelector.opacity = 1;
                                }
                            }
                        }
                    }
                    Label {
                        id: templateName;
                        width: parent.width;
                        horizontalAlignment: Text.AlignHCenter;
                        text: model.text;
                        font: Settings.theme.font("templateLabel");
                        color: "#5b6573";
                    }
                }
            }
        }
    }
}
