# Introduction #
Earlgrey 라이브러리는 기본적으로 Visual Studio 2008 + Feature Pack 조합이면 별도의 작업을 하지 않아도 빌드가 된다. 그러나 구미에 맞게 외부 라이브러리와 연동하거나 성능 및 기능 옵션을 조정할 수도 있다.

# 서브버전 체크아웃/업데이트 #
Earlgrey 소스코드를 내려 받다 보면 또 다시 사용자 계정 정보를 물어본다. 이는 외부 라이브러리인 MSBuild.Community.Tasks 를 참조하기 때문이다. 이때 사용자 이름은 **guest** 로 하고 암호는 비워놓으면 된다.

# BuildConfiguration.h #
