# Описание

Библиотека содержащая класс для микширования 2-х потоков (Pcm::Mixer) и некоторые вспомогательные классы и константы
Решение тестового задания (Задание.pdf)

## Зависимости

- cmake >= 3.9.6
- GoogleTestFramwork

## Сборка

- Сгенерировать релизную сборочную папку

    cmake -H. -BRelease
    
- Собрать проект

    cmake --build Release
    
- Запустить тесты

    cmake --build Release --target test
    
- Очистить проект

    cmake --build Release --target clean
    
- Сгенерировать отладочную сборочную папку

    cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug
