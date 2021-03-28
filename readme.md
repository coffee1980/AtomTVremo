# 何するもの？
- スマホやpcのWEBブラウザから、東芝REGZAのリモコン操作を行う。

# 何が必要？
1. ATOM LITE
2. USB（A-C）ケーブル（※電源なので他で代用可能）

# どうやる？
1. Arduino IDEに、IRremoteESP8266とかのライブラリを準備しておく
2. このスケッチのSSID/PASSを自分ちのものに書き換える
3. Arduino IDEでビルドしてATOM LITEに書き込む
4. ATOM LITEをREGZAのUSBに繋いで給電し、赤外LEDをREGZA受光部に向けて固定する
5. スマホやpcでhttp://m5atom.localにアクセス

![Test Image 3](/image/screen.png)

# ブラウザ表示がしょぼいんですが
すみません
