# fatransfer

Matlab example of face transfer

## Prerequirements

- matlab
- vlfeat
- IntraFace

## HOWTO

Перед запуском вам нужно запустить скрипт `fatransfer_make IMAGE_DIR`,
указав в качестве IMAGE_DIR папку с изображениями лиц в формате jpg.
В папке будет создан служебный файл `faces.mat`.

После этого запустите скрипт `fatransfer IMAGE_DIR` для обработки видеопотока
с камеры.

Или же `fatransfer IMAGE_DIR VIDEO_FILE` для обработки видео файла.