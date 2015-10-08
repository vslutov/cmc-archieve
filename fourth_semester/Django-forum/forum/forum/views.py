from django.shortcuts import render
from django.contrib import auth
from django.contrib.auth.models import User
from django.http import HttpResponseRedirect

from main.models import Message, Thread

def validate_email( email ):
    from django.core.validators import validate_email
    from django.core.exceptions import ValidationError
    try:
        validate_email( email )
        return True
    except ValidationError:
        return False

def register(request):
    if auth.get_user(request).is_authenticated():
        return HttpResponseRedirect('/forum/')
    elif request.method == 'POST':
        email = username = request.POST.get('email', '')
        if not validate_email(email):
            return render(request, 'register.html',
                    {'errors': 'You should enter a valid e-mail'})

        if User.objects.filter(username=username):
            return render(request, 'register.html',
                    {'errors': 'This e-mail is already using'})

        password = request.POST.get('password', '')
        user = User.objects.create_user(username=username, password=password, email=email)

        if user:
            user = auth.authenticate(username=username, password=password)
            auth.login(request,user)
            return HttpResponseRedirect("/forum/")
        else:
            return render(request, 'register.html', {errors: "Can't create user"})
    else:
        return render(request, 'register.html')

def login(request):
    if auth.get_user(request).is_authenticated():
        return HttpResponseRedirect('/forum/')
    elif request.method == 'POST':
        email = request.POST.get('email', '')
        password = request.POST.get('password', '')
        user = auth.authenticate(username=email, password=password)
        if user is not None and user.is_active:
            # Correct password, and the user is marked "active"
            auth.login(request, user)
            # Redirect to a success page.
            return HttpResponseRedirect("/forum/")
        else:
            return render(request, 'login.html', {'errors': 'Wrong e-mail or password'})
    else:
        return render(request, 'login.html')

def logout(request):
    auth.logout(request)
    return HttpResponseRedirect("/forum/")

def forum(request):
    errors = None
    success = None

    if request.method == 'POST':
        text = request.POST.get('text', '')
        user = auth.get_user(request)
        thread = request.POST.get('thread', '')

        if not user.is_authenticated():
            errors = "You should login to post"
        elif len(text) > 140:
            errors = "Max 140 symbols per message"
        else:
            if thread == '':
                thread = Thread()
                thread.save()
            else:
                try:
                    thread = Thread.objects.get(pk=thread)
                except:
                    errors = "Cannot find thread"

            if type(thread) is Thread:
                message = Message(text=text, user=user, thread=thread)
                message.save()
                success = "Message have sent"

    threads = Thread.objects.order_by('-pub_date')
    for thread in threads:
        thread.head = thread.message_set.first()
        thread.tail = thread.message_set.all()[1:]
    return render(request, 'forum.html', {'threads' : threads, 'errors': errors, 'success': success})

def index(request):
    if auth.get_user(request).is_authenticated():
        return HttpResponseRedirect("/forum/")
    else:
        return HttpResponseRedirect("/login/")
