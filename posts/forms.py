from django import forms
from .models import Row

class RowForm(forms.ModelForm):
    class Meta:
        model = Row
        fields = [
              'col1',
              'col2',
              'col3',
              'col4',
              'col5',
              'col6',
              'col7',
              'col8',
              'col9',
        ]
        labels = {
            'col1' : "1",
            'col2' : "2",
            'col3' : "3",
            'col4' : "4",
            'col5' : "5",
            'col6' : "6",
            'col7' : "7",
            'col8' : "8",
            'col9' : "9",
        }