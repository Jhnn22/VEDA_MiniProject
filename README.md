# VEDA_MiniProject
7월 3주차 미니 프로젝트를 c++기반 Qt로 구현한 프로그램입니다.

### [gamemanage.h]
+ 문제로 선택된 임의의 단어와 입력한 단어의 각 문자를 비교해 스트라이크와 볼
을 판단하고 문자열의 길이가 다르면 경고창을 띄웁니다.

#### [gamewidget.ui]
+ "CheckBox"로 단어 주제를 클릭하고, '추가'버튼으로 주제를 추가해줍니다.
아래 "QLineEdit"에 단어를 입력해서, 게임을 진행하면 됩니다.
오른쪽 위 화면에 ID와 Nickname이 표시됩니다. 비회원의 경우 Nickname만 표시됩니다.
오른쪽 아래 화면에 "TextBrowser"로 남은 횟수와 누적 마일리지를 표시합니다.

#### [memberdialog.ui]
+ 왼쪽 화면에 회원가입을 한 닉네임이 "QListview"로 표시됩니다.
"QListView"에 표시된 닉네임을 클릭하면 오른쪽 정보창(QTextBrowser)에 정보가 표시됩니다.
아래 "QLineEdit"창에 닉네임을 검색해서 회원정보를 정보창에 표시합니다.

#### [menuwidget.ui]
+ "게임 시작" 버튼을 누르면 "signindialog.ui"가 켜져 로그인 후에 게임 시작이 가능합니다.
이미 로그인 한 상태면, 추가 로그인 없이 게임 시작이 가능합니다.
"회원 목록" 버튼을 누르면 "memberinfodialog.ui"가 켜져 회원 목록을 확인할 수 있습니다.
"종료" 버튼을 누르면 qApp->quit()됩니다.
"단어 목록" 버튼을 누르면 "wordinfodialog.ui"가 켜져 단어 목록을 확인할 수 있습니다.

#### [signindialog.ui]
+ 회원/비회원으로 두 가지 tab으로 나눴습니다.
회원 tab에서 "ID"와 "PW"를 입력해 "로그인" 버튼을 누르면 게임 시작이 가능합니다.
ID와 PW가 맞지 않으면 로그인이 되지 않습니다.
회원가입버튼을 누르면 "signupdialog.ui"가 켜져 회원가입이 가능합니다.
비회원 tab에서 "QLineEdit"에 ID를 입력하고, 로그인하면 게임 시작이 가능합니다.
회원가입은 위와 같습니다.

#### [signup.ui]
+ NickName과 ID를 입력했을 때, 중복확인을 합니다.
중복확인은 회원 가입을 했을 때, txt로 저장된 회원 목록과 비교를 해서 중복을 걸러냅니다.
비밀번호는 두번 입력을 해서 비밀번호를 잘 입력했는지 확인합니다.
ID와 PW를 입력하여 가입을 하면 member.txt에 회원 정보(NickName, ID, PW)가 저장됩니다.

#### [wordinfodialog.ui]
+ 왼쪽에 주제목록이 나옵니다.
주제목록은 "./theme" 폴더에 저장되어있는 "txt파일"의 이름을 불러옵니다.
오른쪽 정보창에 주제"txt파일"에 들어 있는 내용이 오름차순으로 정렬되어 표시됩니다.
아래 "QLineEdit" 창에 단어를 검색하면, 단어가 포함된 주제 정보가 정보창에 표시됩니다.

