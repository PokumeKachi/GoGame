import QtQuick
import lumieTcg

Text {
    id: root

    enum Type {
        Display,
        Title,
        Heading,
        Body,
        Caption
    }

    property int type: GameText.Type.Body

    font.family: Typography.family
    color: Theme.text
    renderType: Text.QtRendering
    font.hintingPreference: Font.PreferFullHinting

    font.pixelSize: {
        if (!Typography) return 16;

        switch(type) {
        case GameText.Type.Display:
            return Typography.display
        case GameText.Type.Title:
            return Typography.title
        case GameText.Type.Heading:
            return Typography.heading
        case GameText.Type.Body:
            return Typography.body
        case GameText.Type.Caption:
            return Typography.caption
        default:
            return Typography.body
        }
    }
}
