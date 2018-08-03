#coding=utf-8

import os
import sys
import urllib
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.keys import Keys
from lxml import etree
import requests
import time

classes = ["名著", "历史", "经济学", "理财"]

headers = {
        "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.104 Safari/537.36",
        }

def save_img(image_url_list):
    for i, image_url in enumerate(image_url_list):
        image_name = "miner_{:>05}.jpg".format((i))
        with open(save_dir + image_name, "wb") as f:
            content = requests.get(image_url, headers=headers).content
            f.write(content)


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

url = "https://book.douban.com/tag/"

for c in classes:
    kw = urllib.parse.quote(c)

    for i in range(40):
        start = i*20
        
        print(url + kw + "?start={}".format(start))
        driver.get(url + kw + "?start={}".format(start))
        time.sleep(3)

        selector = etree.HTML(driver.page_source)
        subject_item = selector.xpath('//li[contains(@class,"subject-item")]')
        # subject_item = selector.xpath('//li[@class="subject-item"]')
        for item in subject_item:
            # print(item)
            book_cover = item.xpath('.//img/@src')[0]
            # print(book_cover)
            book_name = item.xpath('.//a/@title')[0]

            book_rating = item.xpath('.//span[@class="rating_nums"]')
            if len(book_rating) == 1:
                book_rating = book_rating[0].text
            print("{}: {}".format(book_name, book_rating))

