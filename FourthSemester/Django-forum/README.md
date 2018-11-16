# Django-forum
Учебный Django-форумчик для курса [«Разработка прикладных программ на языке программирования Python» (UNИX, 2015)] (https://uneex.ru/LecturesCMC/PythonDevelopment2015)

## Как запустить локально (virtualenv + pip)

1. Поставить ```virtualenv``` и ```pip```.
1. Склонировать этот репозиторий и перейти в этот каталог (где лежит ```requirements.txt```)
2. Создать virtualenv в текущем каталоге (```virtualenv .```)
3. Активировать этот virtualenv (```. bin/activate```) — в shell prompt появится имя текущего каталога в скобках, например: ```(forum)pavel@pavelt:~/dev/forum$```
4. Внутри него сказать ```pip install -r requirements.txt``` (поставится Django)
5. Сказать ```forum/manage.py migrate``` — создастся и промигрируется локальная БД (```db.sqlite3```)
6. Сказать ```forum/manage.py syncdb``` и ответить на вопросы (email администратора можно указать любой) — создастся пользователь-администратор (для простоты укажите логин admin и пароль admin)
5. Сказать ```forum/manage.py runserver 8001``` — запустится devserver на порту 8001
6. В браузере открыть [```http://0.0.0.0:8001/```](http://0.0.0.0:8001/) или [```http://localhost:8001/```](http://localhost:8001/)
7. ...
8. PROFIT!
