import requests
from bs4 import BeautifulSoup
import os
import sys


module_path = os.path.abspath(os.getcwd())

if module_path not in sys.path:
    sys.path.append(module_path)
    

BASE_URL = "https://www.lyrics.com"

def get_song_and_lyrics(path):
    new_url = BASE_URL + path
    r = requests.get(new_url)
    soup = BeautifulSoup(r.content , 'html.parser')
    return soup.find('h1').text, soup.find('pre').text


url = BASE_URL + "/album/3769520/Now+20th+Anniversary%2C+Vol.+2"

r = requests.get(url)
soup = BeautifulSoup(r.content , 'html.parser')
tags = soup.find_all('strong')

song_links = []
# iterate over each song entry and grab the link to the lyrics
for s in tags:
    link = s.find('a')
    if link and link['href'].startswith('/lyric'):
        song_links.append(link['href'])

songs = {}
# then we iterate over all the lyric links and get the lyrics for each song
# those lyrics are then stored in songs[song_title]
for l in song_links:
    song,lyrics = get_song_and_lyrics(l)
    songs[song] = lyrics
    f = open(song+".txt" ,"w")
    f.write(lyrics)
    f.close()