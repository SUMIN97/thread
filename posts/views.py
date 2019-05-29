from django.shortcuts import render, redirect
from .models import Row
from .forms import RowForm
import pdb
import os
import subprocess
import time

# Create your views here.
def new(request):
    return render(request, 'new.html')
    
def create(request):
    output=0
    if (request.method == 'POST'):
        command1 = './a '
        command2 = ""
        for j in list(range(1,10)):
            row_key = 'row' + str(j) + '[]'
            row_array = request.POST.getlist(row_key)
            i = 0
            for i in list(range(9)):
                command2 += str(row_array[i])
            
        
        #os.system('g++ -o a thread.cpp -pthread')
        

        command1 +=command2
        
        os.system(command1)
        
      
        
        f = open('test.txt', 'r')
        lines = f.readlines()
        result = 0
        cnt = 0
        for line in lines:
            if(cnt == 1):
                break
            else:
                result = line
                cnt = cnt + 1
        f.close()

        if(result == "0\n"):
            return redirect('posts:error')
        
        else:
            return redirect('posts:success')
            
        
def error(request):
    return render(request,'error.html')
    
def success(request):
    return render(request, 'success.html')
    
    

        
        