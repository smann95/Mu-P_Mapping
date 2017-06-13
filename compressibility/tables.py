import sys
import urllib2
from bs4 import BeautifulSoup

link = sys.argv[1]

page = urllib2.urlopen(link)


soup = BeautifulSoup(page, "html.parser")

print soup.prettify()
