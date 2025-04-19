time함수는 1970년 1월 1일 0시부터 경과된 현재까지 몇 초 지났는지를 알려줍니다.

tm 구조체는 다음과 같이 구성되어 있습니다.

struct tm {

        int tm_sec;   //초
        int tm_min;   //분
        int tm_hour;  //시
        int tm_mday;  //일
        int tm_mon;   //월 [0~11]
        int tm_year;  //년 [현재시간 표시시 +1900 필요]
        int tm_wday;  //요일 [0-일요일, 1-월요일, ... ,6-토요일] 
        int tm_yday;  //이번해의 몇번째 일인지
        int tm_isdst; 

};

https://softtone-someday.tistory.com/12
