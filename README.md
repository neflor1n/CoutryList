# CountryList

*Language versions: [English](README.md) | [Русский](README.ru.md) | [Eesti](README.et.md)*

A simple C++ application that fetches data from the REST Countries API and generates an HTML page displaying information about different countries with search functionality.

## Features

- Fetches country data from [REST Countries API](https://restcountries.com/v3.1/all)
- Generates an HTML file with country information including:
  - Country name
  - Flag
  - Region
  - Population
  - Capital
- Search functionality to filter countries by name or capital

## Requirements

- C++17
- CMake 3.10+
- libcurl
- nlohmann_json

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/neflor1n/CoutryList.git
cd CoutryList
```

2. Install dependencies using vcpkg:
```bash
./vcpkg install curl nlohmann-json
```

3. Create a build directory and compile:
```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
make
```

## Usage

1. Run the executable:
```bash
./main
```

2. The program will generate an HTML file named `countries.html`

3. Open the HTML file in your web browser to view the list of countries

4. Use the search bar to filter countries by name or capital

---
*Last updated: 2025-05-12*
