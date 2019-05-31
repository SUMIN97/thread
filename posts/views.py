# render와 redirect 를 사용하기 위해 library 에서 import 해준다.
from django.shortcuts import render, redirect
#os.system 함수를 사용하기 위해 사용한다. 
import os

# new 함수는 화면에 new.html 를 띄워준다
def new(request):
    return render(request, 'new.html')

#create 함수    
def create(request):
    if (request.method == 'POST'):  #new.html 에서 form 요청을 보내는 방식(method)가 POST 라면 
        #os.system 함수를 통해 linux에서 컴파일을 할수 있다.
        #컴파일은 한번만 하고 ./a 뒤에 보내주는 argv에 들어가는 숫자들만 바꿔 linux를 실행한다.
        #os.system('g++ -o a thread.cpp -pthread')      
        command1 = './a ' #linux 컴파일 명령어
        command2 = ""
        for j in list(range(1,10)): #받은 정보가 row1[], row2[] .. row9[]이런형태로 넘어온다
            row_key = 'row' + str(j) + '[]'
            row_array = request.POST.getlist(row_key) #list를 받는다.
            i = 0
            for i in list(range(9)):
                command2 += str(row_array[i])   #list를 string 형태로 바꿔 command2에 이어붙인다.

        command1 +=command2 #command1에 command2를 이어붙인다
        
        os.system(command1) #linux에 command1 명령어를 실행시킨다.
      
        f = open('test.txt', 'r') #CPP 수행시 만들어준 text.txt 파일은 연다
        lines = f.readlines() #파일의 lines를 읽는다.
        result = 0
        cnt = 0
        for line in lines:
            if(cnt == 1):   #첫번째 줄만 읽어온다
                break
            else:
                result = line   #line의 결과를 result에 저장한다.
                cnt = cnt + 1
        f.close()   #읽은 파일을 닫는다

        if(result == "0\n"):    #결과가 0이라면 실패 페이지를 띄운다
            return redirect('posts:error')
        
        else:   #결과가 1이라면 성공 페이지를 띄운다
            return redirect('posts:success')
            
#실패 페이지를 띄우는 함수이다
def error(request):
    return render(request,'error.html')
    
#성공페이지를 듸우는 함수이다.    
def success(request):
    return render(request, 'success.html')
    
    

        
        