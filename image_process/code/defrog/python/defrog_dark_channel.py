#coding=utf-8
'''
何凯明博士的去雾文章和算法实现已经漫天飞了，我今天也就不啰里啰唆， 
直接给出自己 python 实现的完整版本，全部才 60 多行代码，简单易懂，
并有简要注释，去雾效果也很不错。

在这个 python 版本中，计算量最大的就是最小值滤波，纯 python 写的，
慢，可以进一步使用 C 优化，其他部分都是使用 numpy 和 opencv 的现
成东东，效率还行。

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
    '''
        引导滤波，直接参考网上的 matlab 代码
    '''
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
 
def getV1(src, r, eps, w, maxV1):  #输入rgb图像，值范围[0,1]
    ''' 
        计算大气遮罩图像(透视率图) V1 和光照值 A, V1 = 1 - t/A
    '''
    
    # 得到暗通道图像
    V1 = np.min(src, 2) 
    
    # 使用引导滤波优化
    V1 = guidedfilter(V1, zmMinFilterGray(V1,7), r, eps) 
    
    # 计算大气光照 A
    bins = 2000
    ht = np.histogram(V1, bins)    
    d = np.cumsum(ht[0])/float(V1.size) # V1 的均值
    for lmax in range(bins-1, 0, -1):
        if d[lmax] <= 0.999:   # 从暗通道图中按照亮度的大小取前 0.1% 的像素。
            break
    print (np.cumsum(ht[0])[:20])
    print(ht[0])
    A  = np.mean(src, 2)[V1>=ht[1][lmax]].max() # np.mean(src, 2) 是每个像素点三个通道的均值
         
    V1 = np.minimum(V1*w, maxV1)  # 对值范围进行限制
     
    return V1, A
 
def deHaze(src, r=81, eps=0.001, w=0.95, maxV1=0.80, bGamma=False):
    '''
        图像被归一化到 [0-1] 之间.  
        去雾公式 J(x) = ( I(x) - V1 ) / ( 1 - V1/A ) 
    '''

    dst = np.zeros(src.shape)
    V1, A = getV1(src, r, eps, w, maxV1)     # 得到遮罩图像(透视率图)和大气光照
    
    for k in np.arange(3):
        dst[:,:,k] = (src[:,:,k] - V1)/(1-V1/A)  # 颜色校正

    dst =  np.clip(dst, 0, 1)

    if bGamma:
        dst = dst**(np.log(0.5)/np.log(dst.mean()))  # gamma校正,默认不进行该操作
    
    return dst
 
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
    if cv2.waitKey(0):
        cv2.destroyAllWindows()
    # cv2.imwrite(image[:image.rfind('.')]+"_out.png", result)
