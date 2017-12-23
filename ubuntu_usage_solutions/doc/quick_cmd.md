# linux 实用命令

## 1. `ls` 实现列文件按时间排序    
```bash
# 时间最近的在前面
ls -lt  
# 时间从前到后
ls -ltr 
# 利用sort
ls -l | sort +7      # 时间从前到后(日期为第8列)
ls -l | sort -r +7   # 时间最近的在前面
```
## 2. find 命令和其他命令组合    
2.1 特定文件中的字符串替换   
(1) 正则表达式   
以下命令将所有 `.xml` 文件中的 `jst0330_002194` 字符串替换为 `jst`。    
```
$ find Annotations/ -name '*.xml' |xargs perl -pi -e 's|<name>jst[0-9][0-9][0-9][0-9]_[0-9][0-9][0-9][0-9][0-9][0-9]</name>|<name>jst</name>|g'
```
(2) 字符串完全匹配   
```bash
$ find -name '*.xml' |xargs perl -pi -e 's|<name>st</name>|<name>xwk</name>|g'
```
2.2 操作的文件数量太多:`Argument list too long`。     
```bash
$ find xml/ -name "*.xml" -exec cp {} Annotations/ \;
$ find jpg/ -name "*.jpg" -exec cp {} JPEGImages/ \;
```
## 3. 使用 nohup 生成 log 文件   
```bash
nohup python examples/ssd/ssd_xwk.py > xwk.log 2>&1 &
```
