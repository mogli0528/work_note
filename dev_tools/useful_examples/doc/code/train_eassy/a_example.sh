错误文件夹	图片错误状态	正确的label
jst068	        full	  jst086
jst120	        part	  jst167
jst167	        part	  jst120
jst261	        part	  jst262
jst264	        part	  jst286

# jst068  [0011_00464-00011_02460]
# jst120  [0088_00000-0088_02000]   作废样本: 0088_01060-0088_01540,   0262_00000, 0262_00108
# jst068  [00464-02460]
# jst068  [00464-02460]
# jst068  [00464-02460]
# jst068  [00464-02460]

作废样本
jst_multi_0262_00884-jst_multi_0262_01048
jst_multi_0088_00596-jst_multi_0088_00624
jst_multi_0088_01552-jst_multi_0088_01836
jst_multi_0262_00000-jst_multi_0262_00108
jst_multi_0262_00892-jst_multi_0262_00992

find jst120/ -name "jst_multi_0262_0[0][8-9][8-9][4-9].xml" -exec rm {} \; && \
find jst120/ -name "jst_multi_0262_010[0-4][0-8].xml" -exec rm {} \; && \
find jst120/ -name "jst_multi_0088_0059[6-9].xml" -exec rm {} \; && \
find jst120/ -name "jst_multi_0088_01[5-7][0-9][0-9].xml" -exec rm {} \; && \
find jst120/ -name "jst_multi_0088_018[0-3][0-6].xml" -exec rm {} \; && \
find jst120/ -name "jst_multi_0262_000[0-9][0-9].xml" -exec rm {} \; && \
find jst120/ -name "jst_multi_0262_0010[0-8].xml" -exec rm {} \;



find jpg/ -name "jst_multi_0262_0[0][8-9][8-9][4-9].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0262_010[0-4][0-8].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0088_0059[6-9].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0088_01[5-7][0-9][0-9].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0088_018[0-3][0-6].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0262_0089[2-9].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0262_009[0-9][0-2].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0262_009[0-8][0-9].jpg"  -exec rm {} \; && \
find jpg/ -name "jst_multi_0262_0099[0-2].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0262_000[0-9][0-9].jpg" -exec rm {} \; && \
find jpg/ -name "jst_multi_0262_0010[0-8].jpg" -exec rm {} \;

find xml/ -name "jst_multi_0262_0[0][8-9][8-9][4-9].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0262_010[0-4][0-8].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0088_0059[6-9].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0088_01[5-7][0-9][0-9].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0088_018[0-3][0-6].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0262_0089[2-9].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0262_009[0-9][0-2].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0262_009[0-8][0-9].xml"  -exec rm {} \; && \
find xml/ -name "jst_multi_0262_0099[0-2].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0262_000[0-9][0-9].xml" -exec rm {} \; && \
find xml/ -name "jst_multi_0262_0010[0-8].xml" -exec rm {} \;



find -name "jst_multi_0011_0([0][4-9][6-9][4-9])|([1][0-9][0-9][0-9])|([2][0-4][0-6][0]).xml"

find -regextype "posix-egrep" -regex  "jst_multi_0011_0[0][4-9][6-9][4-9]|jst_multi_0011_0[1][0-9][0-9][0-9]|jst_multi_0011_0[2][0-4][0-6][0].xml"


 -name pattern
              Base  of  file  name  (the  path  with  the  leading  directories removed) matches shell pattern pattern.  The
              metacharacters (`*', `?', and `[]') match a `.' at the start of the base name (this  is  a  change  in  findu‐
              tils-4.2.2;  see  section  STANDARDS  CONFORMANCE  below).   To ignore a directory and the files under it, use
              -prune; see an example in the description of -path.  Braces are not recognised as being special,  despite  the
              fact  that  some  shells  including  Bash imbue braces with a special meaning in shell patterns.  The filename
              matching is performed with the use of the fnmatch(3) library function.   Don't forget to enclose  the  pattern
              in quotes in order to protect it from expansion by the shell.



# 提取错误 xml 文件
# jst068 [0011_00464-00011_02460]
find xml/ -name "jst_multi_0011_0[0][4-9][6-9][4-9].xml" -exec cp {} error/ \; && \
find xml/ -name "jst_multi_0011_0[1][0-9][0-9][0-9].xml" -exec cp {} error/ \; && \
find xml/ -name "jst_multi_0011_0[2][0-4][0-5][0-9].xml" -exec cp {} error/ \; && \
find xml/ -name "jst_multi_0011_0460.xml" -exec cp {} error/ \;
# 更正为正确的 name
find error/ -name '*.xml' |xargs perl -pi -e 's|<name>jst068</name>|<name>jst086</name>|g'   
mv error/*.xml xml/


# jst120 -> jst167
-0088_02000
find jst_multi/ -name "jst_multi_0088_0[0][0-9][0-9][0-9].xml" -exec cp {} error \; && \
find jst_multi/ -name "jst_multi_0088_0[1][0-9][0-9][0-9].xml" -exec cp {} error \; && \
find jst_multi/ -name "jst_multi_0088_02000.xml" -exec cp {} error \;
find error/ -name '*.xml' |xargs perl -pi -e 's|<name>jst167</name>|<name>jst1677</name>|g'   
find error/ -name '*.xml' |xargs perl -pi -e 's|<name>jst120</name>|<name>jst167</name>|g'   
find error/ -name '*.xml' |xargs perl -pi -e 's|<name>jst1677</name>|<name>jst120</name>|g'   
mv error/*.xml xml/

# jst261 -> jst262
# jst_multi_0295_00000 - jst_multi_0295_01996
find xml/ -name "jst_multi_0295_0[0][0-9][0-9][0-9].xml" -exec cp {} error \; && \
find xml/ -name "jst_multi_0295_0[1][0-9][0-8][0-9].xml" -exec cp {} error \; && \
find xml/ -name "jst_multi_0295_0199[0-6].xml" -exec cp {} error \; 
find error/ -name '*.xml' |xargs perl -pi -e 's|<name>jst261</name>|<name>jst262</name>|g' 
mv error/*.xml xml/

# jst264 -> jst286
# jst_multi_02222_00000 - jst_multi_02222_02012
find xml/ -name "jst_multi_02222_0[0][0-9][0-9][0-9].xml" -exec cp {} error \; && \
find xml/ -name "jst_multi_02222_0[1][0-9][0-9][0-9].xml" -exec cp {} error \; && \
find xml/ -name "jst_multi_02222_0200[0-9].xml" -exec cp {} error \; && \
find xml/ -name "jst_multi_02222_0201[0-2].xml" -exec cp {} error \; 
find error/ -name '*.xml' |xargs perl -pi -e 's|<name>jst264</name>|<name>jst286</name>|g' 
find error/ -name 'jst_multi_02222_02004.xml' |xargs perl -pi -e 's|<name>jst264</name>|<name>jst286</name>|g' 
find error/ -name 'jst_multi_02222_02008.xml' |xargs perl -pi -e 's|<name>jst264</name>|<name>jst286</name>|g' 

mv error/*.xml xml/



# 1. 以下命令将`jst0330_002194`字符串替换为`jst`。    
```
find Annotations/ -name '*.xml' |xargs perl -pi -e 's|<name>jst[0-9][0-9][0-9][0-9]_[0-9][0-9][0-9][0-9][0-9][0-9]</name>|<name>jst</name>|g' && \
find Annotations/ -name '*.xml' |xargs perl -pi -e 's|<name>jst[0-9][0-9][0-9]</name>|<name>jst</name>|g'
```