from django.urls import path

from . import views

app_name = 'dbs'
urlpatterns = [
    path('v1/health/', views.v1Health, name='health'),
    path('v1/ov/submissions/', views.v1Submissions, name='submissions'),
    path('v1/ov/submissions/<int:delete_id>/', views.v1Submissions_delete, name='delete_submissions'),
    path('v1/companies/', views.v1companies, name='companies'),
    path('v2/ov/submissions/', views.v2Submissions, name='submissions2'),
    path('v2/ov/submissions/<int:number>/', views.v2Submissions_int, name='v2_with_number')
]
