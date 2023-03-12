<h1 align="center">Привет! Я Евгений
<img src="https://github.com/blackcater/blackcater/raw/main/images/Hi.gif" height="32"/></h1>
<h3 align="center">Начинающий программист и разработчик игр</h3>

<br><h1 align="center">Поисковый движок</br>
<h1 align="left"> :pencil2: Описание проекта:</h1>

<br><p align="justify">Реализация собственного поискового движка, который поможет осуществлять поиск среди набора документов.</p></br>

<h1 align="left"> :wrench: Описание стека используемых технологий : </h1>
<div>
  <img src="https://github.com/devicons/devicon/blob/master/icons/cplusplus/cplusplus-original.svg" title="C++" alt="C++" width="60" height="60"/>&nbsp;
  <img src="https://github.com/devicons/devicon/blob/master/icons/cmake/cmake-original.svg" title="cmake" alt="cmake" width="60" height="60"/>&nbsp;
  <img src="https://github.com/devicons/devicon/blob/master/icons/jetbrains/jetbrains-original.svg" title="jetbrains" alt="jetbrains" width="60" height="60"/>&nbsp; 
  <img src="https://github.com/devicons/devicon/blob/master/icons/windows8/windows8-original.svg" title="windows" alt="windows" width="60" height="60"/>&nbsp;   
</div>


<br><h1 align="left"> :notebook: Краткая инструкция, как запустить проект локально:</h1></br>

<p> :black_circle: Создаём текстовые файлы по которым программа будет осуществлять поиск. </p>

<p> :black_circle: Json файлы "config.json", "answers.json" и "requests.json" по умолчанию находятся в директории с проектом. Нужно заполнить файлы "config.json" и "answers.json" </p>

<p> Пример заполнения "config.json":  </p>

```Json
{
  "config": {
    "max_responses": 5,
    "name": "SkillboxSearchEngine",
    "version": "0.1"
  },
  "files": [
    "1.txt",
    "2.txt",
    "3.txt",
    "4.txt",
    "5.txt",
    "6.txt"
  ]
}
```

<p> Пример заполнения "requests.json":  </p>

```Json
{
  "requests": [
    "moscow is the capital of russia",
    "moscow",
    "on"
  ]
}
```
<p> :black_circle: После заполнения .txt и Json файлов запускаем программу. </p>

<p> :black_circle: При успешном завершении программы появится надпись "Successfully! The result is in file "result.html"" </p>

<p> :black_circle: Результат можно посмотреть в файле "answers.json" и для визуализации в файле "result.html" </p>