# Game development using GYAGEY

## File structure
```console
|-- game.json
`-- resources
    |-- fonts
    |-- images
    `-- music
```
This is recommended by devs games file tree structure. Hovewer any file structure can be used as long as it fits in JSON file structure.

## Json format

As for now, json support is really weak, it has a couple of constrains:
 - No comments.
 - No integers, only strings.
 - You cannot change order of objects, for example "MenuBg" cannot be before "Name".

If you want no characters displayed for current scene, put 'NONE' in scene characters list.

```json
{
    "Game" : {
        "Name"      : "<Game Title>",
        "MenuBg"    : "<path to menu background png>",
        "MenuMusic" : "<path to menu music {mp3, wav}>",
        "<character 1 name>" : "<path to character1 png>",
        "<character 2 name>" : "<path to character2 png>",
        "<character 3 name>" : "<path to character3 png>",
        ...
        "<character N name>" : "<path to characterN png>",
    },
    "Scenes" : {
        "1" : { "Background" : "<path to scene background png>",
                "Music"      : "<path to scene music file{mp3, wav}>",
                "Characters" : ["<character 1 name>", "<character 2 name>"...],
                "Frases"     : {
                    "<character 1 name>" : "<voice line>"
                    "<character 3 name>" : "<voice line>"
                    "<character 2 name>" : "<voice line>"
                    "CHOISE":{
                        "<scene id>" : "Choice text 1",
                        "<scene id>" : "choice text 2",
                        "0": "0"
                    },
                    "END":"END"
                }
            },
        "2" : { "Background" : "<path to scene background png>",
                "Music"      : "<path to scene music file{mp3, wav}>",
                "Characters" : ["NONE"],
                "Frases"     : {
                    "NONE" : "<voice line>"
                    "NONE" : "<voice line>"
                    "NONE" : "<voice line>"
                    "GOTO" : "<scene id>"
                    "END":"END"
                }
            }
        "3" : { "Background" : "<path to scene background png>",
                "Music"      : "<path to scene music file{mp3, wav}>",
                "Characters" : ["NONE"],
                "Frases"     : {
                    "NONE" : "<voice line>"
                    "NONE" : "<voice line>"
                    "NONE" : "<voice line>"
                    "GOTO" : "<scene id>"
                    "END":"THE END"
                }
        }
    }
}
```

# Разработка игр под ГОЙДА

## Русский

## Структура дерева файлов

```console
|-- game.json
`-- resources
    |-- fonts
    |-- images
    `-- music
```

Такая структура рекомендованна разработчиками ГОЙДы. Однако любая структура подходит, пока она соответствует структуре JSON файла.

## формат Json файла

На данный момент поддержка Json очень огранничена:
 - Не поддерживаются комментарии.
 - Только строковые объекты.
 - Порядок объектов не может быть изменен, например "MenuBg" не может находиться до "Name".

Если вы не хотите отображат персонажей в данной сцене, укажите в списке 'Characters' только "NONE".

```json
{
    "Game" : {
        "Name"      : "<Название игры>",
        "MenuBg"    : "<путь к png фона меню>",
        "MenuMusic" : "<путь к музыке в меню {mp3, wav}>",
        "<имя персонажа 1>" : "<путь к изображению персонажа 1>",
        "<имя персонажа 2>" : "<путь к изображению персонажа 2>",
        "<имя персонажа 3>" : "<путь к изображению персонажа 3>",
        ...
        "<имя персонажа N>" : "<путь к изображению персонажа N>",
    },
    "Scenes" : {
        "1" : { "Background" : "<путь к изображению фона сцены>",
                "Music"      : "<путь к фоновой музыке{mp3, wav}>",
                "Characters" : ["<имя персонажа 1>", "<имя персонажа 2>", ...],
                "Frases"     : {
                    "<имя персонажа 1>" : "<реплика>"
                    "<имя персонажа 3>" : "<реплика>"
                    "<имя персонажа 2>" : "<реплика>"
                    "END":"END"
                }
            },
        "2" : { "Background" : "<путь к изображению фона сцены>",
                "Music"      : "<путь к фоновой музыке{mp3, wav}>",
                "Characters" : ["NONE"],
                "Frases"     : {
                    "NONE" : "<реплика>"
                    "NONE" : "<реплика>"
                    "NONE" : "<реплика>"
                    "END":"END"
                }
        }
    }
}
```


