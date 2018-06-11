#coding=utf-8
'''
何凯明博士的去雾文章和算法实现已经漫天飞了，我今天也就不啰里啰唆，直接给出自己 python 实现的完整版本，全部才 60 多行代码，简单易懂，并有简要注释，去雾效果也很不错。

在这个 python 版本中，计算量最大的就是最小值滤波，纯 python 写的，慢，可以进一步使用 C优化，其他部分都是使用 numpy 和 opencv 的现成东东，效率还行。
'''

import cv2
import numpy as np
import argparse
 
def zmMinFilterGray(src, r=7):
    ''' 最小值滤波，r是滤波器半径
        实现策略是: 递归方式, 每次一行一列的 kernel 做最小值
    '''
    if r <= 0:
        return src
    h, w = src.shape[:2]
    I = src

    res = np.minimum(I, I[[0]+[x for x in range(h-1)], :])  # 顶部复制 Bouning
    res = np.minimum(res, I[[x for x in range(1,h)]+[h-1], :])  # 底部复制 Bouning
    I = res
    # 列比较
    res = np.minimum(I, I[:, [0]+[x for x in range(w-1)]])
    res = np.minimum(res, I[:, [x for x in range(1,w)]+[w-1]])

    return zmMinFilterGray(res, r-1)  
 
def guidedfilter(I, p, r, eps):
    '''引导滤波，直接参考网上的 matlab 代码'''
    height, width = I.shape
    m_I = cv2.boxFilter(I, -1, (r,r))
    m_p = cv2.boxFilter(p, -1, (r,r))
    m_Ip = cv2.boxFilter(I*p, -1, (r,r))
    cov_Ip = m_Ip-m_I*m_p
 
    m_II = cv2.boxFilter(I*I, -1, (r,r))
    var_I = m_II-m_I*m_I
 
    a = cov_Ip/(var_I+eps)
    b = m_p-a*m_I
 
    m_a = cv2.boxFilter(a, -1, (r,r))
    m_b = cv2.boxFilter(b, -1, (r,r))
    return m_a*I+m_b
 
def getV1(m, r, eps, w, maxV1):  #输入rgb图像，值范围[0,1]
    '''计算大气遮罩图像 V1 和光照值 A, V1 = 1-t/A'''
    V1 = np.min(m,2)                                         #得到暗通道图像
    V1 = guidedfilter(V1, zmMinFilterGray(V1,7), r, eps)     #使用引导滤波优化
    bins = 2000
    ht = np.histogram(V1, bins)                              #计算大气光照A
    d = np.cumsum(ht[0])/float(V1.size)
    for lmax in range(bins-1, 0, -1):
        if d[lmax]<=0.999:
            break
    A  = np.mean(m,2)[V1>=ht[1][lmax]].max()
         
    V1 = np.minimum(V1*w, maxV1)                   # 对值范围进行限制
     
    return V1,A
 
def deHaze(m, r=81, eps=0.001, w=0.95, maxV1=0.80, bGamma=False):
    Y = np.zeros(m.shape)
    V1,A = getV1(m, r, eps, w, maxV1)     # 得到遮罩图像和大气光照
    for k in np.arange(3):
        Y[:,:,k] = (m[:,:,k]-V1)/(1-V1/A) # 颜色校正
    Y =  np.clip(Y, 0, 1)
    if bGamma:
        Y = Y**(np.log(0.5)/np.log(Y.mean()))  # gamma校正,默认不进行该操作
    return Y
 
if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('-i', '--image', required = True)
    ap.add_argument('-o', '--output', required = True)
    args = vars(ap.parse_args())

    image = args["image"]
    output = args["output"]
    print(image, output)

    result = deHaze(cv2.imread(image)/255.0)*255
    cv2.imwrite(output, result)
    cv2.imshow("result", result.astype(np.uint8))
    cv2.waitKey(0)
    # cv2.imwrite(image[:image.rfind('.')]+"_out.png", result)
