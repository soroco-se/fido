This is fido, a tool to find, identify, date and organize image and movie files.
Hopefully in an automatic fashion.

Find Identify Date Organize

Dependencies:
 - libmagic
 - libextractor
    https://www.gnu.org/software/libextractor/manual/libextractor.html
    - gstreamer, with plugins
    - libgstreamer
 - libexiv2

Diary:
Dåså, traversering och id funkar (isch)
Har hittat verktyg/lib som kan plocka metadata ur filmer (mediainfo). Har byggt libbet under third_party
Har inte integrerat det än, inte heller libexif som behövs för bilderna.


Hur ska jag hantera ingorerade kataloger? (mama_backup, Håkan-backup etc?)
