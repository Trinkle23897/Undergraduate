import os, time
from bs4 import BeautifulSoup as bs
from selenium import webdriver
from selenium.webdriver.chrome.options import Options

if __name__ == "__main__":
    chrome_options = Options()
    chrome_options.add_argument("--headless") # comment for looking its behavior
    driver = webdriver.Chrome(chrome_options=chrome_options)
    driver.get("http://166.111.121.62:9000/login")
    driver.find_element_by_name("username").send_keys("[replace it with username]")
    driver.find_element_by_name("password").send_keys("[replace it with password]")
    driver.find_element_by_xpath("(.//*[normalize-space(text()) and normalize-space(.)='Password:'])[1]/following::button[1]").click()
    rejudges = [10593]
    cnt = 0 
    total = 0 
    t = time.time()
    while True:
        try:
            driver.get("http://166.111.121.62:9000/dashboard/")
            text = bs(driver.page_source, 'html.parser').text
            if 'Pending' in text:
                t = time.time()
            elif not 'Running' in text and not 'Pending' in text:
                print('use %lf' % (time.time() - t)) 
                total += 1
                driver.get("http://166.111.121.62:9000/restart_submission/%d/" % rejudges[cnt])
                #driver.get('http://166.111.121.62:9000/mark_submission/%d/' % rejudges[cnt])
                print('%s #%d: %d' % (time.asctime()[4:-5], total, rejudges[cnt]), end=', ')
                t = time.time()
                cnt = (cnt + 1) % len(rejudges)
        except:
            pass
        os.system('sleep 1')
