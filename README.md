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
  
  > <h3>5. 각 센서별 사용할 GPIO PIN 번호 최종 선택 및 정리<h3/>
  ```
  👉 각 센서별 GPIO PIN 번호 설정
  
  ⦁ 센서
  - LED: LED1(Blue)-20번, LED2(Red)-21번
  - UART(Bluetooth): RXD-0번, TXD-1번
  - 조도 센서: SDA-2번, SCL-3번
  - 초음파 센서: TRIG-15번, ECHO-18번
  - 로터리 엔코더: CLK-19번, DT-26번
  - 7 segment display(4-digit): 4번, 7번, 8번, 9번, 10번, 13번, 17번, 22번, 23번, 24번, 25번, 27번
  ```
- 프로젝트 메인 코드에 관한 자세한 내용은 repository에 첨부된 『<strong>main.c</strong>』 파일 참조

</br>

## 4. 데모 영상 촬영 및 편집✔
- 데모 영상 촬영 및 편집 날짜/시간 : 2022.12.10(토) / 15:00~18:00
- 최종 데모 영상 링크 : https://youtu.be/PWtGpSawjvg

</br>

## 5. 최종 발표 자료 준비✔
- 최종 발표 자료 준비 날짜/시간 : 2022.12.11(일) / 13:00~15:00
- 발표 내용 : 아이디어 소개, 전체 시스템 구조, 제한조건 구현, 기술 활용도, 문제점 및 해결방안, 데모 영상
- 최종 발표에 관한 자세한 내용은 repository에 첨부된 『<strong>발표자료(최종).pptx</strong>』 파일 참조

</br>

## 6. 최종 데모 준비✔
- 최종 데모 준비 날짜/시간 : 2022.12.19(월) / 20:00~20:30
- 수행 내용 : 선 연결 재확인 및 센서 동작 여부 테스트

</br>

## 최종 정리✔
   > <h3>1. 일정 및 준비 사항<h3/>
  ```
  👉 위 본문에 작성된 일련의 개발 과정 참고
  
  - ✅ 프로젝트 아이디어 회의
  - ✅ 아이디어 제안 발표
  - ✅ 샘플 코드 수집 및 회로도 조사 & 프로젝트 메인 코드 구현
  - ✅ 데모 영상 촬영 및 편집
  - ✅ 최종 발표 자료 준비
  - ✅ 최종 데모 준비
  ```
  
   > <h3>2. 주제<h3/>
  ```
  👉 실생활에 활용가능한 임베디드 시스템 (자유 주제)
  👉 다른 수업에서 배웠던 내용/기술 등 접목 적극 권장
  
  - 자유 주제 
    : 스마트 주차장(Motivated by. 주차장 알림등)
  
  - 접목 내용/기술 
    : 『IoT』과목 및 특강에서 사용해 본 'Segment Display'를 활용함
    : 『운영체제』과목에서 배운 멀티 쓰레드, 상호배제를 위한 뮤텍스(Mutex)를 사용함
  ```
  
   > <h3>3. 제한 조건<h3/>
  ```
  👉 쓰레드 또는 멀티프로세싱 필수 사용
  👉 IPC 또는 뮤텍스 필수 사용
  
  - 멀티 쓰레드(Multi Thread) 사용 -> 5개의 센서를 동시 구동(th1 ~ th5)
  - 뮤텍스(Mutex) 사용 -> 각 건물별로 남은 주차자리 배열 순차 계산(Digital, Global, Library)
  ```
  
   > <h3>4. 추가 기술 활용<h3/>
  ```
  👉 기술 활용도 및 통신 사용 여부
  
  - 총 6개의 센서 사용(LED, UART, 조도 센서, 초음파 센서, 로터리 엔코더, 7 segment display) 
  - ADC 활용 : 조도 센서를 사용하여 각 건물별 차량 주차 여부 확인
  - 직렬 통신 활용 : UART(Bluetooth) 통신을 통한 각 건물별 실시간 남은 주차자리 수 확인
  - 통신 사용 : 라즈베리파이(Raspberry Pi) - 스마트폰(Smart Phone) -> UART(Bluetooth)
  
  ⭐ 조도 센서 + 초음파 센서 사용 : 밤낮 상관없이 주차 여부 판단 가능 -> 주차 인식에 대한 안정성 향상
  ```

</br>

## 최종 결과물✔
   > <h3>전체 시스템 구조<h3/>
   <p align="left">
    <img src="https://user-images.githubusercontent.com/74342121/208481008-8fb6176a-d10d-492e-9386-f9585d452fa2.gif" width="400" height="250">
    <img src="https://user-images.githubusercontent.com/74342121/208481518-4072ca84-0cbe-4329-994a-da458d95c63f.gif" width="400" height="250">
   </p>
   
   <br/>
   
   > <h3>기술 활용도<h3/>
   <p align="left">
    <img src="https://user-images.githubusercontent.com/74342121/208481029-1bb09485-3889-40d0-88be-04763d14713c.png" width="420" height="220">
   </p>
   
   <br/>

   > <h3>최종 결과물<h3/>
   <p align="left">
    <img align="center" width="380" height="230" src="https://user-images.githubusercontent.com/74342121/208485371-dd895ef2-83a1-49b0-8f45-906f18a3bf55.jpg">&nbsp;&nbsp;&nbsp;👉&nbsp;&nbsp;&nbsp;<img align="center" width="380" height="230" src="https://user-images.githubusercontent.com/74342121/208481035-2bb37f4b-4c2a-40fe-9f88-81a7867859f4.png">
   </p>
   


   
   
   
   
   
   
