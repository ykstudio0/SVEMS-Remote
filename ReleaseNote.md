v0.1.1
    SET 진입
    WiFi Setup
    English / Korean
    현재 선택 언어 * 표시
    언어 즉시 전환
    NVS 저장
    재부팅 후 언어 유지

v0.1.2 Vehicle Charge

v0.1.3
    Remote Brightness
    - Manual 밝기 설정 구조
    - NVS 저장/복원
    - 0~100% → 0~255 변환
    - 부팅 시 저장값 적용
    - Settings Menu에 DISPLAY 추가
    - DISPLAY SETTINGS 화면 진입
    - 현재 50% 값 표시
    **2026-09-04**
    Remote MANUAL 밝기 5~100%, 5% 단위
    SAVE / CANCEL + NVS 저장/복원
    AUTO / MANUAL 모드 저장
    AUTO에서 수동 조절 비활성화
    LDR GPIO4 입력 + IIR 필터
    AUTO 밝기 단계 계산 + STABLE 판정
    Fade In / Fade Out 부드럽게 적용
    AUTO 현재 적용 밝기 실시간 표시
    값 변경 시에만 redraw
    HTTPS Telemetry Worker 분리
    HTTP timeout 중에도 터치/화면/AGE 정상 유지
    DISPLAY SETTINGS 한글 표시 정상화