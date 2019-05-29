from django.contrib import admin
from django.urls import path
from . import views

app_name = "posts"
urlpatterns = [
    path('new/', views.new, name = "new"),
    path('create/', views.create, name = "create"),
    path('success/', views.success, name = "success"),
    path('error/',views.error, name = "error"),
]