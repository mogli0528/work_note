#coding=utf-8

import os
import urllib
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.keys import Keys
from lxml import etree
import requests



headers = {
        "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.104 Safari/537.36",
        }

# headers = {'User-Agent': "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36"}

home_dir = os.getenv("HOME")
save_dir = "{}/work/spider/miners/".format(home_dir) 
print(save_dir)

if not os.path.exists(save_dir):
    os.makedirs(save_dir)

chrome_options = Options()
chrome_options.add_argument('--headless')
chrome_options.add_argument('--disable-gpu')
driver = webdriver.Chrome(chrome_options=chrome_options)

url = "https://image.baidu.com/search/index?tn=baiduimage&word="

kw = "矿工"
kw = urllib.parse.quote(kw)

driver.get(url + kw)
selector = etree.HTML(driver.page_source)
image_url_list = selector.xpath('//div[@class="imgbox"]//@data-imgurl')
print(len(image_url_list))

for i, image_url in enumerate(image_url_list):
    print(i, image_url)
    image_name = "{:>05}.jpg".format((i))
    with open(save_dir + image_name, "wb") as f:
        content = requests.get(image_url, headers=headers).content
        f.write(content)
