# Embedded_System_Team_Project
임베디드시스템 team project를 위한 개발 Repository입니다.
> <h3>팀 : RTX408조(8조)<h3/>

</br>

## 1. 프로젝트 아이디어 회의✔
- 회의 날짜/시간 : 2022.11.29(화) / 18:00~19:30
- 회의 내용 : 프로젝트 아이디어 제안, 시스템 구조 설계, 센서 활용 방안, 개발 일정, 업무 분장
- 회의 결론 : "제 3안. 스마트 주차장" 채택✅
- 회의에 관한 자세한 내용은 repository에 첨부된 『<strong>회의록(아이디어 제안).docx</strong>』 파일 참조

</br>

## 2. 아이디어 제안 발표✔
- 발표 날짜/시간 : 2022.12.02(금) / 15:00~16:00
- 발표 내용 : 채택한 아이디어 주제, 시스템 구조 소개, 개발 일정 및 업무 분장
- 아이디어 제안 발표에 관한 자세한 내용은 repository에 첨부된 『<strong>발표자료(아이디어 제안).pptx</strong>』 파일 참조

</br>

## 3. 샘플 코드 수집 및 회로도 조사 & 프로젝트 메인 코드 구현✔
- 코드 수집 및 회로도 조사 날짜/시간 : 2022.12.05(월) / 19:00~21:00
- 프로젝트 메인 코드 구현(1차) 날짜/시간 : 2022.12.06(화) / 10:00~12:00
- 프로젝트 메인 코드 구현(2차) 날짜/시간 : 2022.12.07(수) / 20:00~22:00
- 프로젝트 메인 코드 구현(3차) 날짜/시간 : 2022.12.09(금) / 19:00~22:00
- 프로젝트 메인 코드 구현(최종) 날짜/시간 : 2022.12.10(토) / 13:00~15:00

- 수행 내용
  > <h3>1. 샘플 코드 수집 및 회로도 조사<h3/>
  ```
  👉 팀원별로 1~2가지 센서를 선택하여 c언어 소스코드 수집
  👉 강의자료와 인터넷 등을 참고하여 회로도 조사
  
  ⦁ 센서
  - LED: LED.c
  - UART(Bluetooth): UART.c
  - 조도 센서: Photoresistor.c
  - 초음파 센서: Ultrasonic.c
  - 로터리 엔코더: Rotary_encoder.c
  - 7 segment display(4-digit): Segment_display.c
  - Thread: Thread.c
  - Thread mutex: Thread_mutex.c
  
  ⦁ 참고자료
  - https://learn.sunfounder.com/lesson-18-rotary-encoder
  - https://www.electronicshub.org/raspberry-pi-ultrasonic-sensor-interface-tutorial
  - https://wiki.keyestudio.com/KS0221_keyestudio_Ultimate_Starter_Kit_for_Raspberry_Pi#Lesson_15:_4-digit_LED_Segment_Display
  ```
  
  > <h3>2. 프로젝트 메인 코드 구현(1차)<h3/>
  ```
  👉 수집한 샘플 코드를 기반으로 프로젝트 메인 코드 구현
  
  - [LED, UART(Bluetooth), 조도 센서, 초음파 센서] 제어 코드 및 쓰레드(Thread) 구현
  - 코드 구현 후, 각각의 센서 작동 여부 확인: 이상 무
  ```
  
  > <h3>3. 프로젝트 메인 코드 구현(2차)<h3/>
  ```
  👉 수집한 샘플 코드를 기반으로 프로젝트 메인 코드 구현
  
  - [로터리 엔코더, 7 segment display(4-digit)] 제어 코드 및 디스플레이 값 출력 구현
  - 쓰레드(Thread): 3개 -> 5개로 증가 = LED 센서를 제외한 5개의 센서 제어 함수를 쓰레드화 시킴
  - 코드 구현 후, 각각의 센서 작동 여부 확인: 이상 무
  ```
  
   > <h3>4. 프로젝트 메인 코드 구현(3차)<h3/>
  ```
  👉 이전 코드를 기반으로 프로젝트 메인 코드 구현 
  
  - 클린 코드(Clean Code) 수행 -> 함수 이름 재정의, 주석 작성, 불필요한 변수 제거, 코드 구조 변경 등
  - 코드 구현 후, 각각의 센서 작동 여부 확인: 이상 무
  ```
  
  > <h3>4. 프로젝트 메인 코드 구현(최종)<h3/>
  ```
  👉 이전 코드를 기반으로 프로젝트 메인 코드 구현
  
  - 뮤텍스(Mutex) 코드 추가
  - 각 센서에서 사용할 GPIO PIN 번호 최종 선택 및 정리
  - 클린 코드(Clean Code) 2차 수행 -> 주석 작성(세분화), 불필요한 변수 제거, 코드 최종 검토 등
  - 코드 구현 후, 각각의 센서 작동 여부 확인: 이상 무
  ```
- 프로젝트 메인 코드에 관한 자세한 내용은 repository에 첨부된 『<strong>main.c</strong>』 파일 참조

</br>

## 4. 데모 영상 촬영 및 편집✔
- 데모 영상 촬영 및 편집 날짜/시간 : 2022.12.10(토) / 15:00~18:00
- 최종 데모 영상 링크 : https://youtu.be/PWtGpSawjvg
