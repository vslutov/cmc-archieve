from django.db import models
from django.contrib.auth.models import User
from md5 import md5
from django.utils import timezone
from date_diff import date_diff

class Thread(models.Model):
    pub_date = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return str(self.message_set.first())

class Message(models.Model):
    text = models.TextField()
    user = models.ForeignKey(User, null=True, blank=True)
    pub_date = models.DateTimeField(default=timezone.now)
    thread = models.ForeignKey(Thread)

    def __init__(self, *args, **kwargs):
        super(Message, self).__init__(*args, **kwargs)
        self.gravatar = "http://www.gravatar.com/avatar/" + md5(self.user.email).hexdigest()
        self.human_time = date_diff(self.pub_date)

    def __str__(self):
        return self.text
