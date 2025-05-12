# CountryList

*Keelevalikud: [English](README.md) | [Русский](README.ru.md) | [Eesti](README.et.md)*

Lihtne C++ rakendus, mis toob andmeid REST Countries API-st ja genereerib HTML-lehe, kuvades informatsiooni erinevate riikide kohta koos otsingufunktsiooniga.

## Funktsioonid

- Toob riikide andmed [REST Countries API](https://restcountries.com/v3.1/all) kaudu
- Genereerib HTML-faili riikide informatsiooniga, sealhulgas:
  - Riigi nimi
  - Lipp
  - Piirkond
  - Rahvaarv
  - Pealinn
- Otsingufunktsioon riikide filtreerimiseks nime või pealinna järgi

## Nõuded

- C++17
- CMake 3.10+
- libcurl
- nlohmann_json

## Projekti ehitamine

1. Klooni hoidla:
```bash
git clone https://github.com/neflor1n/CoutryList.git
cd CoutryList
```

2. Paigalda sõltuvused vcpkg abil:
```bash
./vcpkg install curl nlohmann-json
```

3. Loo ehituskataloog ja kompileeri:
```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=tee/vcpkg/scripts/buildsystems/vcpkg.cmake
make
```

## Kasutamine

1. Käivita rakendus:
```bash
./main
```

2. Programm genereerib HTML-faili nimega `countries.html`

3. Ava HTML-fail veebilehitsejas, et näha riikide loendit

4. Kasuta otsinguriba riikide filtreerimiseks nime või pealinna järgi

---
*Viimati uuendatud: 2025-05-12*
