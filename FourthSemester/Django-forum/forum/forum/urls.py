from django.conf.urls import patterns, include, url
from django.contrib import admin

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'forum.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    url(r'^register/', 'forum.views.register'),
    url(r'^login/', 'forum.views.login'),
    url(r'^logout/', 'forum.views.logout'),

    url(r'^$', 'forum.views.index'),
    url(r'^forum/', 'forum.views.forum'),
    url(r'^admin/', include(admin.site.urls)),
)
