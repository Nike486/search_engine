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


<p> :black_circle: Объявляем классы:</p>

```C++
ConverterJSON converterJSON; 
InvertedIndex invertedIndex; 
SearchServer searchServer(invertedIndex);
```


<p> :black_circle: В фигурные скобки пишем запросы: </p>

```C++
converterJSON.setRequests(std::vector<std::string>{"moscow is the capital of russia", "moscow", "on"});
```


<p> :black_circle: В фигурные скобки пишем тексты по которым будет осуществляться поиск: </p>


```C++
invertedIndex.UpdateDocumentBase(std::vector<std::string>{"london is the capital of great britain",
                                                          "paris is the capital of france",
                                                          "berlin is the capital of germany",
                                                          "rome is the capital of italy",
                                                          "madrid is the capital of spain",
                                                          "lisboa is the capital of portugal",
                                                          "bern is the capital of switzerland",
                                                          "moscow is the capital of russia",
                                                          "kiev is the capital of ukraine",
                                                          "minsk is the capital of belarus",
                                                          "astana is the capital of kazakhstan",
                                                          "beijing is the capital of china",
                                                          "tokyo is the capital of japan",
                                                          "bangkok is the capital of thailand",
                                                          "welcome to moscow the capital of russia the third rome",
                                                          "amsterdam is the capital of netherlands",
                                                          "helsinki is the capital of finland",
                                                          "oslo is the capital of norway",
                                                          "stockholm is the capital of sweden",
                                                          "riga is the capital of latvia",
                                                          "tallinn is the capital of estonia",
                                                          "warsaw is the capital of poland"});
```



<p> :black_circle: Функция записи результатов в файл "answers.json.": </p>

```C++
converterJSON.putAnswers(searchServer.completionAnswers());    
```


<p> :black_circle: После запуска проекта в файле "answers.json." запишутся результаты поиска. </p>
