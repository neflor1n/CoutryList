#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Функция для записи данных из cURL
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    std::string apiUrl = "https://restcountries.com/v3.1/all";
    std::string response;

    // Инициализация cURL
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Выполнение запроса
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }

        // Завершение работы cURL
        curl_easy_cleanup(curl);
    }

    // Парсинг JSON
    auto countries = nlohmann::json::parse(response);

    // Создаем HTML файл
    std::ofstream htmlFile("countries.html");
    htmlFile << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
    htmlFile << "<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    htmlFile << "<title>Country List</title>\n<style>\n";
    htmlFile << "body { font-family: Arial, sans-serif; background: #f4f4f4; padding: 1rem; }\n";
    htmlFile << ".search-container { margin-bottom: 20px; }\n";
    htmlFile << "#searchInput { padding: 10px; width: 300px; border-radius: 5px; border: 1px solid #ddd; }\n";
    htmlFile << ".country { background: white; margin: 1rem 0; padding: 1rem; border-radius: 5px; box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); }\n";
    htmlFile << ".country img { width: 100px; height: auto; border-radius: 5px; margin-right: 1rem; }\n";
    htmlFile << ".country h2 { margin: 0; }\n";
    htmlFile << ".hidden { display: none; }\n";
    htmlFile << "</style>\n</head>\n<body>\n";
    
    // Добавляем заголовок и поисковую строку
    htmlFile << "<h1>Country List</h1>\n";
    htmlFile << "<div class=\"search-container\">\n";
    htmlFile << "  <input type=\"text\" id=\"searchInput\" placeholder=\"Search by country or capital...\">\n";
    htmlFile << "</div>\n";
    
    // Создаем контейнер для стран
    htmlFile << "<div id=\"countriesContainer\">\n";

    // Генерация HTML-контента
    for (const auto& country : countries) {
        std::string name = country["name"]["common"].get<std::string>();
        std::string flag = country["flags"]["png"].get<std::string>();
        std::string region = country.contains("region") ? country["region"].get<std::string>() : "N/A";
        std::string population = std::to_string(country["population"].get<int>());
        std::string capital = country.contains("capital") ? country["capital"][0].get<std::string>() : "N/A";

        // Создаем data-атрибуты для поиска
        htmlFile << "<div class=\"country\" data-name=\"" << name << "\" data-capital=\"" << capital << "\">\n";
        htmlFile << "<img src=\"" << flag << "\" alt=\"" << name << " flag\">\n";
        htmlFile << "<h2>" << name << "</h2>\n";
        htmlFile << "<p><strong>Region:</strong> " << region << "</p>\n";
        htmlFile << "<p><strong>Population:</strong> " << population << "</p>\n";
        htmlFile << "<p><strong>Capital:</strong> " << capital << "</p>\n";
        htmlFile << "</div>\n";
    }
    
    htmlFile << "</div>\n"; // Закрываем контейнер

    // Добавляем JavaScript для функции поиска
    htmlFile << "<script>\n";
    htmlFile << "document.getElementById('searchInput').addEventListener('keyup', function() {\n";
    htmlFile << "  const searchValue = this.value.toLowerCase();\n";
    htmlFile << "  const countries = document.getElementsByClassName('country');\n";
    htmlFile << "  \n";
    htmlFile << "  for (let i = 0; i < countries.length; i++) {\n";
    htmlFile << "    const country = countries[i];\n";
    htmlFile << "    const name = country.getAttribute('data-name').toLowerCase();\n";
    htmlFile << "    const capital = country.getAttribute('data-capital').toLowerCase();\n";
    htmlFile << "    \n";
    htmlFile << "    if (name.includes(searchValue) || capital.includes(searchValue)) {\n";
    htmlFile << "      country.classList.remove('hidden');\n";
    htmlFile << "    } else {\n";
    htmlFile << "      country.classList.add('hidden');\n";
    htmlFile << "    }\n";
    htmlFile << "  }\n";
    htmlFile << "});\n";
    htmlFile << "</script>\n";

    htmlFile << "</body>\n</html>";
    htmlFile.close();

    std::cout << "HTML файл создан: countries.html" << std::endl;

    return 0;
}