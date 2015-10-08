# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.utils.timezone
from django.conf import settings


def add_threads_to_messages(apps, schema_editor):
    db_alias = schema_editor.connection.alias
    Thread = apps.get_model("main", "Thread")
    Message = apps.get_model("main", "Message")
    User = apps.get_model("auth", "User")

    messages = Message.objects.using(db_alias).all()
    if len(messages) > 0:
        author = None
        for user in User.objects.using(db_alias).all():
            if user.is_superuser:
                author = user
                break
        if not author:
            raise Exception("Couldn't find superuser. Please, add it.")

        for message in messages:
            thread = Thread.objects.using(db_alias).create()
            thread.save()
            message.thread = thread
            message.user = author
            message.save()


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('main', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Thread',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('pub_date', models.DateTimeField(default=django.utils.timezone.now)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.AddField(
            model_name='message',
            name='pub_date',
            field=models.DateTimeField(default=django.utils.timezone.now),
            preserve_default=True,
        ),
        migrations.AddField(
            model_name='message',
            name='thread',
            field=models.ForeignKey(default=0, to='main.Thread'),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='message',
            name='user',
            field=models.ForeignKey(to=settings.AUTH_USER_MODEL, null=True),
            preserve_default=True,
        ),
        migrations.RunPython(
            add_threads_to_messages,
        ),
    ]
