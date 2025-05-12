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
    htmlFile << "* { box-sizing: border-box; margin: 0; padding: 0; }\n";
    htmlFile << "body { font-family: 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif; background: #f5f7fa; color: #333; line-height: 1.6; padding: 0; }\n";
    htmlFile << "header { background: linear-gradient(135deg, #1e5799, #207cca); color: white; padding: 2rem; text-align: center; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n";
    htmlFile << "h1 { font-size: 2.5rem; margin-bottom: 1rem; font-weight: 300; }\n";
    htmlFile << ".container { max-width: 1200px; margin: 0 auto; padding: 2rem; }\n";
    htmlFile << ".search-container { background: white; padding: 1.5rem; border-radius: 8px; margin-bottom: 2rem; box-shadow: 0 4px 6px rgba(0,0,0,0.05); }\n";
    htmlFile << "#searchInput { width: 100%; padding: 12px 20px; font-size: 1rem; border: 1px solid #ddd; border-radius: 30px; transition: all 0.3s; box-shadow: inset 0 1px 3px rgba(0,0,0,0.1); }\n";
    htmlFile << "#searchInput:focus { outline: none; border-color: #207cca; box-shadow: 0 0 0 3px rgba(32,124,202,0.2), inset 0 1px 3px rgba(0,0,0,0.1); }\n";
    htmlFile << ".countries-grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(350px, 1fr)); gap: 1.5rem; }\n";
    htmlFile << ".country { background: white; border-radius: 8px; overflow: hidden; transition: transform 0.3s, box-shadow 0.3s; box-shadow: 0 4px 6px rgba(0,0,0,0.05); }\n";
    htmlFile << ".country:hover { transform: translateY(-5px); box-shadow: 0 12px 20px rgba(0,0,0,0.1); }\n";
    htmlFile << ".country-header { position: relative; height: 160px; overflow: hidden; background: #eee; }\n";
    htmlFile << ".country-header img { width: 100%; height: 100%; object-fit: cover; }\n";
    htmlFile << ".country-body { padding: 1.5rem; }\n";
    htmlFile << ".country h2 { font-size: 1.5rem; margin-bottom: 1rem; color: #333; }\n";
    htmlFile << ".country-info { display: flex; flex-direction: column; gap: 0.5rem; }\n";
    htmlFile << ".country-info p { margin: 0; }\n";
    htmlFile << ".info-item { display: flex; align-items: center; }\n";
    htmlFile << ".info-label { font-weight: 600; min-width: 100px; color: #555; }\n";
    htmlFile << ".info-value { color: #333; }\n";
    htmlFile << ".hidden { display: none; }\n";
    htmlFile << ".no-results { text-align: center; padding: 3rem; font-size: 1.2rem; color: #666; grid-column: 1 / -1; }\n";
    htmlFile << "footer { text-align: center; padding: 2rem; margin-top: 2rem; font-size: 0.9rem; color: #777; }\n";
    htmlFile << "@media (max-width: 768px) { .countries-grid { grid-template-columns: 1fr; } header { padding: 1.5rem; } h1 { font-size: 2rem; } }\n";
    htmlFile << "</style>\n</head>\n<body>\n";
    htmlFile << "<header>\n";
    htmlFile << "  <h1>World Countries Explorer</h1>\n";
    htmlFile << "  <p>Discover information about countries around the world</p>\n";
    htmlFile << "</header>\n";
    htmlFile << "<div class=\"container\">\n";
    htmlFile << "  <div class=\"search-container\">\n";
    htmlFile << "    <input type=\"text\" id=\"searchInput\" placeholder=\"Search by country or capital...\">\n";
    htmlFile << "  </div>\n";
    htmlFile << "  <div class=\"countries-grid\" id=\"countriesContainer\">\n";

    // Генерация HTML-контента для каждой страны
    for (const auto& country : countries) {
        std::string name = country["name"]["common"].get<std::string>();
        std::string flag = country["flags"]["png"].get<std::string>();
        std::string region = country.contains("region") ? country["region"].get<std::string>() : "N/A";
        std::string population = std::to_string(country["population"].get<int>());
        std::string capital = country.contains("capital") ? country["capital"][0].get<std::string>() : "N/A";

        // Форматирование числа населения с разделителями
        std::string formattedPopulation = population;
        int insertPosition = formattedPopulation.length() - 3;
        while (insertPosition > 0) {
            formattedPopulation.insert(insertPosition, ",");
            insertPosition -= 3;
        }

        // Создаем data-атрибуты для поиска
        htmlFile << "<div class=\"country\" data-name=\"" << name << "\" data-capital=\"" << capital << "\">\n";
        htmlFile << "  <div class=\"country-header\">\n";
        htmlFile << "    <img src=\"" << flag << "\" alt=\"" << name << " flag\">\n";
        htmlFile << "  </div>\n";
        htmlFile << "  <div class=\"country-body\">\n";
        htmlFile << "    <h2>" << name << "</h2>\n";
        htmlFile << "    <div class=\"country-info\">\n";
        htmlFile << "      <div class=\"info-item\"><span class=\"info-label\">Capital:</span> <span class=\"info-value\">" << capital << "</span></div>\n";
        htmlFile << "      <div class=\"info-item\"><span class=\"info-label\">Region:</span> <span class=\"info-value\">" << region << "</span></div>\n";
        htmlFile << "      <div class=\"info-item\"><span class=\"info-label\">Population:</span> <span class=\"info-value\">" << formattedPopulation << "</span></div>\n";
        htmlFile << "    </div>\n";
        htmlFile << "  </div>\n";
        htmlFile << "</div>\n";
    }
    
    // Закрываем контейнер и добавляем подвал
    htmlFile << "  </div>\n"; // Закрываем countries-grid
    htmlFile << "</div>\n"; // Закрываем контейнер

    // Добавляем JavaScript для функции поиска
    htmlFile << "<script>\n";
    htmlFile << "document.getElementById('searchInput').addEventListener('keyup', function() {\n";
    htmlFile << "  const searchValue = this.value.toLowerCase();\n";
    htmlFile << "  const countries = document.getElementsByClassName('country');\n";
    htmlFile << "  let visibleCount = 0;\n";
    htmlFile << "  \n";
    htmlFile << "  for (let i = 0; i < countries.length; i++) {\n";
    htmlFile << "    const country = countries[i];\n";
    htmlFile << "    const name = country.getAttribute('data-name').toLowerCase();\n";
    htmlFile << "    const capital = country.getAttribute('data-capital').toLowerCase();\n";
    htmlFile << "    \n";
    htmlFile << "    if (name.includes(searchValue) || capital.includes(searchValue)) {\n";
    htmlFile << "      country.classList.remove('hidden');\n";
    htmlFile << "      visibleCount++;\n";
    htmlFile << "    } else {\n";
    htmlFile << "      country.classList.add('hidden');\n";
    htmlFile << "    }\n";
    htmlFile << "  }\n";
    htmlFile << "  \n";
    htmlFile << "  // Показываем сообщение если ничего не найдено\n";
    htmlFile << "  const noResults = document.getElementById('noResults');\n";
    htmlFile << "  if (noResults) {\n";
    htmlFile << "    noResults.remove();\n";
    htmlFile << "  }\n";
    htmlFile << "  \n";
    htmlFile << "  if (visibleCount === 0 && searchValue !== '') {\n";
    htmlFile << "    const container = document.getElementById('countriesContainer');\n";
    htmlFile << "    const noResultsMsg = document.createElement('div');\n";
    htmlFile << "    noResultsMsg.id = 'noResults';\n";
    htmlFile << "    noResultsMsg.className = 'no-results';\n";
    htmlFile << "    noResultsMsg.textContent = 'No countries found matching \"' + searchValue + '\"';\n";
    htmlFile << "    container.appendChild(noResultsMsg);\n";
    htmlFile << "  }\n";
    htmlFile << "});\n";
    htmlFile << "</script>\n";

    // Добавляем подвал
    htmlFile << "<footer>\n";
    htmlFile << "  <p>Created by " << "Bogdan Sergachev, " << "Vsevolod Tsarev, " << "Martin Sild" << " | Data provided by <a href=\"https://restcountries.com\">REST Countries API</a> | " << "2025-05-12" << "</p>\n";
    htmlFile << "</footer>\n";

    htmlFile << "</body>\n</html>";
    htmlFile.close();

    std::cout << "HTML файл создан: countries.html" << std::endl;

    return 0;
}