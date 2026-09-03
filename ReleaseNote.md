v0.1.1
    SET 진입
    WiFi Setup
    English / Korean
    현재 선택 언어 * 표시
    언어 즉시 전환
    NVS 저장
    재부팅 후 언어 유지
v0.1.2
v0.1.3 LCD Brightness
    MANUAL / AUTO 선택
    → MODE 터치로 전환
    → SAVE 시 NVS 저장
    → 재부팅 후 선택한 MODE 유지

    MANUAL 밝기
    → 5~100%
    → 5% 단위
    → 실시간 Preview
    → SAVE 저장
    → CANCEL 원복
    → 재부팅 유지
    1. AUTO 모드일 때 BRIGHTNESS +/- 비활성화
    2. 조도센서 연결 후 AUTO 밝기 제어 구현
    MANUAL
    → 5~100%
    → 5% 단위 조절
    → 실시간 Preview
    → SAVE 시 NVS 저장
    → CANCEL 시 기존값 복귀

    AUTO
    → MODE 저장/복원
    → 수동 밝기값은 유지
    → +/- 입력 무시
    → 화면에서도 비활성 표시
    3.3V
    │
    [LDR]
    │
    ├──────── GPIO4
    │
    [10kΩ]
    │
    GND
    가림     200~500
    실내    2500~3500
    강한 빛 3900+
    
    ADC <= 1900  → 5%
    ADC >= 3900  → 100%
    그 사이      → 5~100% 변환