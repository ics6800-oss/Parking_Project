# FPGA 초음파 차단기 제어 시스템

## 개요
BASYS3 기반 초음파 거리 측정 차단기 제어 시스템

## 주요 기능
- HC-SR04 초음파 센서 거리 측정
- 거리 5cm 이하 시 FND에 "STOP" 표시
- 거리 값 4자리 FND 실시간 출력
- 거리 단계별 LED 점등
- 10cm 이하 접근 시 서보 모터 차단기 하강
- 15cm 이상 이탈 시 차단기 상승
- 중간값 + 이동 평균 필터 적용으로 거리 안정화

## 하드웨어
- BASYS3 FPGA 보드
- HC-SR04 초음파 센서
- 서보 모터 (SG90)
- 4자리 7-Segment (FND)
- LED ×16
- AXI GPIO ×4
- AXI Timer ×1

## 파일 구조
- main.c            : 메인 루프 및 시스템 초기화
- fnd.c / fnd.h     : FND 제어 및 STOP 모드 표시
- led.c / led.h     : 거리 기반 LED 제어
- servo.c / servo.h : 서보 PWM 및 차단기 제어
- ultrasonic.c / ultrasonic.h : 초음파 거리 측정 및 필터링
