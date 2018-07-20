
使用vim脚本来实现的，使用了模板,几行代码就能实现很实用。

~/.vimrc 中的代码

~~~vim
" auto add pyhton header {
autocmd BufNewFile *.py 0r ~/.vim/vim_template/vim_pyhton_header
autocmd BufNewFile *.py ks|call FileName()|'s
autocmd BufNewFile *.py ks|call CreatedTime()|'s

fun FileName()
    if line("$") > 10
        let l = 10  "这里是字母L 不是数字1 
    else
        let l = line("$")
    endif 
    exe "1," . l . "g/File Name:.*/s/File Name:.*/File Name: " .expand("%")  
       "最前面是数字1，这里的File Name: 要和模板中一致
endfun 

fun CreatedTime()
    if line("$") > 10
        let l = 10
    else
        let l = line("$")
    endif 
    exe "1," . l . "g/Created Time:.*/s/Created Time:.*/Created Time: " .strftime("%Y-%m-%d %T") 
        "这里Create Time: 要和模板中一致
endfun 

" }
~~~

模板代码 文件为~/.vim/vim_template/vim_pyhton_header  
  
~~~python
#!/usr/bin/python3
#-*- coding:utf-8 -*-
############################
# File Name:
# Author: 
# Mail: klm200x11@gmail.com
# Created Time:  
############################

~~~

说明   

1 模板路径要一致
2 不要忘了保持 .vimrc 中替换标签名称和模板中一致 例如  Created Time:

测试结果
lzz@ubuntu:~$ vim c.py
lzz@ubuntu:~$ cat c.py 


#!/usr/bin/python3
#-*- coding:utf-8 -*-
############################
# File Name: a.py
# Author: klm
# Mail: klm200x11@gmail.com
# Created Time: 2018-07-19 10:57:56
############################

