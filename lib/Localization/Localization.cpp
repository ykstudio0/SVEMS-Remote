//-------------------------------------------------------------
// File : Localization.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-08-25
// Project : SVEMS Remote
// Version : 0.1.1
// Description : Localization
//-------------------------------------------------------------

#include "Localization.h"
#include <Preferences.h>

namespace
{
    constexpr const char* NVS_NAMESPACE = "display";
    constexpr const char* NVS_KEY_LANGUAGE = "language";
}

namespace Localization
{
    static Language g_language =
        Language::English;

    static DataSource g_dataSource =
        DataSource::Main;

    Language GetLanguage()
    {
        return g_language;
    }

    void SetLanguage(
        Language language)
    {
        g_language =
            language;
    }
    
    DataSource GetDataSource()
    {
        return g_dataSource;
    }

    void SetDataSource(
        DataSource dataSource)
    {
        g_dataSource =
            dataSource;
    }

    const char* Get(
        const Text& text,
        Language language)
    {
        if (language == Language::Korean)
        {
            return text.korean;
        }

        return text.english;
    }

    const char* Get(
        const Text& text)
    {
        return Get(
            text,
            g_language);
    }

    // 한글화 대상
    // Header
    const Text HeaderOverview =
    {
        "OVERVIEW",
        "개요"
    };

    const Text HeaderSolar =
    {
        "SOLAR",
        "태양광"
    };

    const Text HeaderBattery =
    {
        "BATTERY",
        "배터리"
    };

    const Text HeaderLoad =
    {
        "LOAD",
        "부하"
    };

    const Text HeaderTemperature =
    {
        "TEMPERATURE",
        "온도"
    };

    const Text HeaderSystem =
    {
        "REMOTE SYSTEM",
        "시스템"
    };

    const Text HeaderSystemDetail =
    {
        "MAIN SYS",
        "메인시스템"
    };
    
    // Contents
    // OverView Main
    const Text OverViewSolar =
    {
        "Solar",
        "태양광"
    };
    
    const Text OverViewBatRemain =
    {
        "Bat Remain",
        "잔여용량"
    };

    const Text OverViewSOC =
    {
        "SOC",
        "충전율"
    };

    // OverView 수정
    // const Text OverViewBatteryPower =
    // {
    //     "Bat Power",
    //     "Bat전력"
    // };

    const Text OverViewBatteryCurrent =
    {
        "Bat Current",
        "Bat전류"
    };

    const Text OverViewLoad =
    {
        "Load",
        "부하"
    };

    const Text OverViewCabin =
    {
        "Cabin",
        "실내온도"
    };

    const Text OverViewHumidity =
    {
        "Humidity",
        "실내습도"
    };

    //Solar Main
    const Text PvVoltage =
    {
        "Pv Voltage",
        "Pv전압"
    };

    const Text PvCurrent =
    {
        "Pv Current",
        "Pv전류"
    };

    const Text PvPower =
    {
        "Pv Power",
        "Pv전력"
    };

    const Text PvStage =
    {
        "Stage",
        "충전단계"
    };

    const Text PvInput =
    {
        "Input",
        "입력상태"
    };

    const Text PvDailyEng =
    {
        "Daily Eng",
        "일일발전"
    };

    // Battery Main
    const Text BatterySOC =
    {
        "SOC",
        "충전율"
    };

    const Text BatteryRemaining =
    {
        "Remaining",
        "잔여용량"
    };

    const Text BatteryPower =
    {
        "Power",
        "전력"
    };

    const Text BatteryCurrent =
    {
        "Current",
        "전류"
    };

    const Text BatteryTemp =
    {
        "Temp",
        "온도"
    };

    const Text BatteryStatus =
    {
        "Status",
        "상태"
    };

    // Load Main
    const Text LoadVoltage =
    {
        "Voltage",
        "전압"
    };

    const Text LoadCurrent =
    {
        "Current",
        "전류"
    };

    const Text LoadPower =
    {
        "Power",
        "전력"
    };

    // Temperature Main
    const Text TemperatureCabin =
    {
        "Cabin",
        "실내"
    };

    const Text TemperatureBattery =
    {
        "Battery",
        "배터리"
    };

    const Text TemperatureBMS =
    {
        "BMS",
        "BMS"
    };

    const Text TemperatureController =
    {
        "Controller",
        "컨트롤러"
    };

    // Battery Dtl(2)
    const Text BMSTemp =
    {
        "BMS Temp",
        "BMS온도"
    };

    const Text ExtTemp =
    {
        "Ext Temp",
        "외부온도"
    };

    const Text Capacity =
    {
        "Capacity",
        "총용량"
    };

    const Text DtlRemaining =
    {
        "Remaining",
        "잔여용량"
    };

    const Text DtlSOC =
    {
        "SOC",
        "충전잔량"
    };

    const Text Runtime =
    {
        "RUNTIME",
        "가용시간"
    };

    // Battery Dtl(1)
    const Text DtlVoltage =
    {
        "Voltage",
        "전압"
    };

    const Text Cell1 =
    {
        "Cell1",
        "셀 1"
    };

    const Text Cell2 =
    {
        "Cell2",
        "셀 2"
    };

    const Text Cell3 =
    {
        "Cell3",
        "셀 3"
    };

    const Text Cell4 =
    {
        "Cell4",
        "셀 4"
    };

    const Text Delta =
    {
        "Delta",
        "셀편차"
    };

    const Text DtlRSSI =
    {
        "RSSI",
        "RSSI"
    };

    const Text DtlEpever =
    {
        "EPEVER",
        "EPEVER"
    };

    const Text DtlBMS =
    {
        "BMS",
        "BMS"
    };

    const Text DtlDevices =
    {
        "Devices",
        "장치"
    };

    const Text DtlUptime =
    {
        "Uptime",
        "가동시간"
    };

    const Text DtlHttp =
    {
        "HTTP",
        "HTTP"
    };

    // System Main
    const Text DtlSysWifi =
    {
        "WIFI",
        "WiFi"
    };

    const Text DtlSysRSSI =
    {
        "RSSI",
        "RSSI"
    };

    const Text DtlSysHeap =
    {
        "HEAP",
        "힙사용량"
    };

    const Text DtlSysUptime =
    {
        "UPTIME",
        "가동시간"
    };

    const Text DtlSysTelemetry =
    {
        "TELEMETRY",
        "원격전송"
    };

    const Text DtlSysAge =
    {
        "AGE",
        "수신경과"
    };

    // System Dtl(2)
    const Text DtlHttpState =
    {
        "HTTP STATE",
        "웹상태"
    };

    const Text DtlHttpOk =
    {
        "HTTP OK",
        "웹성공"
    };

    const Text DtlHttpFail =
    {
        "HTTP FAIL",
        "웹실패"
    };

    const Text DtlHttpConsec =
    {
        "HTTP CONSEC",
        "연속실패"
    };

    const Text DtlHttpMax =
    {
        "HTTP MAX",
        "최대연속"
    };

    const Text DtlHttpError =
    {
        "HTTP ERROR",
        "최근오류"
    };

    // Sysem Dtl(3)
    const Text DtlSolarOff =
    {
        "SOLAR OFF",
        "PV끊김"
    };

    const Text DtlChargeOff =
    {
        "CHARGE OFF",
        "충전끊김"
    };

    const Text DtlBMSOff =
    {
        "BMS OFF",
        "BMS끊김"
    };

    const Text DtlLoadOff =
    {
        "LOAD OFF",
        "부하끊김"
    };

    const Text DtlCtrlOff =
    {
        "CTRL OFF",
        "제어끊김"
    };

    const Text DtlSOCOff =
    {
        "SOC OFF",
        "SOC끊김"
    };

    // System Dtl(4)
    const Text DtlBootCount =
    {
        "BOOT COUNT",
        "부팅횟수"
    };

    const Text DtlRstReason =
    {
        "RST REASON",
        "리셋원인"
    };

    const Text DtlSysVehicleBat =
    {
        "Vehicle Bat",
        "차량전압"
    };

    const Text DtlSysReverseChg =
    {
        "Reverse Chg",
        "역충전"
    };

    // Footer
    const Text FooterPrev =
    {
        "< Prev",
        "< 이전"
    };

    const Text FooterNext =
    {
        "Next >",
        "다음 >"
    };

    // Settings UI
    const Text SettingsTitle =
    {
        "SETTINGS",
        "설정"
    };

    const Text SettingsExit =
    {
        "EXIT",
        "나가기"
    };

    const Text SettingsWiFi =
    {
        "WiFi Setup",
        "WiFi설정"
    };

    const Text SettingsEnglish =
    {
        "English",
        "영어"
    };

    const Text SettingsKorean =
    {
        "Korean",
        "한국어"
    };

    const Text WiFiConfirmTitle =
    {
        "WiFi Setup",
        "WiFi설정"
    };

    const Text WiFiConfirmMessage =
    {
        "Start setup mode?",
        "설정모드 시작?"
    };

    const Text WiFiConfirmCancel =
    {
        "CANCEL",
        "취소"
    };

    const Text WiFiConfirmOk =
    {
        "OK",
        "확인"
    };
}

