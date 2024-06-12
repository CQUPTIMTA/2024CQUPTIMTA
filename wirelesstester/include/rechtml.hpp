/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-06-12 16:01:20
 */
const char* rechtml = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>起重机调试网页</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: flex-end;
            align-items: center;
            height: 100vh;
            flex-direction: row;
        }
        .container {
            position: relative;
            width: 55vw;
            height: 55vw;
            background-color: #acacac;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);
            border-radius: 5%;
        }
        .circle {
            position: absolute;
            width: 5%;
            height: 5%;
            background-color: white;
            border-radius: 50%;
            display: flex; 
            justify-content: 
            center; align-items: center;
            font-size: 16;
            color: black;
        }
        .unit_text{
            position: absolute;
            display: flex; 
            justify-content: 
            center; align-items: center;
            font-size: 16;
            color: red;
        }
        .zero-circle {
            width: 10%;
            height: 10%;
            background-color: white;
        }

        .data {
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            height: 90vh;
            margin-right: 20px;
            font-size: 14px;
        }
        .unit {
            background-color: #ffffff;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            padding: 20px;
            text-align: center;
            width: 300px;
            height: 9vh;
            margin-bottom: 5px;
        }
        .warp {
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            height: 30vh;
            margin-right: 20px;
        }
        button {
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 10px;
            cursor: pointer;
            margin-bottom: 10px;
            width: 100%;
            font-size: 16px;
            height: 50px;
        }

        button:hover {
            background-color: #45a049;
        }
        .rectangle{
            position: absolute;
            width: 5%;
            height: 5%;
            background-color: rgb(236, 197, 106);
            border-radius: 10%;
            
        }
    </style>
</head>
<body>
    <div class="warp">
        <button onclick="window.location.href='/'">主页面</button>
        <button onclick="test()">更新数据</button>
        <button onclick="window.location.href='/func'">测试</button>
    </div>
    <div class="data">
        <div class="unit">
            <label for="ID-11" class="label" data-id="11">ID: 11</label>
            <div>D：<span id="RUdataF"></span>mm</div>
            <div>D：<span id="RUdataB"></span>mm</div>
            <div>V：<span id="Voltage"></span></div>
        </div>
        <div class="unit">
            <label for="ID-12" class="label" data-id="12">ID: 12</label>
            <div>D：<span id="RUdataF"></span>mm</div>
            <div>D：<span id="RUdataB"></span>mm</div>
            <div>V：<span id="Voltage"></span></div>
        </div>
        <div class="unit">
            <label for="ID-13" class="label" data-id="13">ID: 13</label>
            <div>D：<span id="RUdataF"></span>mm</div>
            <div>D：<span id="RUdataB"></span>mm</div>
            <div>V：<span id="Voltage"></span></div>
        </div>
        <div class="unit">
            <label for="ID-14" class="label" data-id="14">ID: 14</label>
            <div>D：<span id="RUdataF"></span>mm</div>
            <div>D：<span id="RUdataB"></span>mm</div>
            <div>V：<span id="Voltage"></span></div>
        </div>
        <div class="unit">
            <label for="ID-15" class="label" data-id="15">ID: 15</label>
            <div>D：<span id="RUdataF"></span>mm</div>
            <div>D：<span id="RUdataB"></span>mm</div>
            <div>V：<span id="Voltage"></span></div>
        </div>
        <div class="unit">
            <label for="ID-16" class="label" data-id="16">ID: 16</label>
            <div>D：<span id="RUdataF"></span>mm</div>
            <div>D：<span id="RUdataB"></span>mm</div>
            <div>V：<span id="Voltage"></span></div>
        </div>
    </div>

    <div class="container">
        <div class="rectangle" style="top: 0; right: 0; transform: translate(-100px, 100px);"></div>
        <div class="rectangle" style="bottom: 0 ;right: 0; transform: translate(-100px, -100px);"></div>
        <div class="rectangle" style="top: 50%; left: 50%; transform: translate(-50%, -50%)"></div>
        <div class="circle zero-circle" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(0deg) translate(-250px) rotate(0deg);"><span>原点</span></div>
        <!-- Inner circles -->
        <div class="circle 7" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(0deg) translate(75px) rotate(0deg);"><span>7</span></div>
        <div class="circle 10" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(60deg) translate(75px) rotate(-60deg);"><span>10</span></div>
        <div class="circle 9" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(120deg) translate(75px) rotate(-120deg);"><span>9</span></div>
        <div class="circle 6" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(180deg) translate(75px) rotate(-180deg);"><span>6</span></div>
        <div class="circle 3" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(240deg) translate(75px) rotate(-240deg);"><span>3</span></div>
        <div class="circle 4" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(300deg) translate(75px) rotate(-300deg);"><span>4</span></div>
        
        <!-- Outer circles -->
        <div class="circle 8" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(0deg) translate(150px) rotate(0deg);"><span>8</span></div>
        <div class="circle 12" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(60deg) translate(150px) rotate(-60deg);"><span>12</span></div>
        <div class="circle 11" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(120deg) translate(150px) rotate(-120deg);"><span>11</span></div>
        <div class="circle 5" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(180deg) translate(150px) rotate(-180deg);"><span>5</span></div>
        <div class="circle 1" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(240deg) translate(150px) rotate(-240deg);"><span>1</span></div>
        <div class="circle 2" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(300deg) translate(150px) rotate(-300deg);"><span>2</span></div>

        <!-- Outer circles -->
        <div class="unit_text" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(0deg) translate(112px) rotate(0deg);"><span>14</span></div>
        <div class="unit_text" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(60deg) translate(112px) rotate(-60deg);"><span>15</span></div>
        <div class="unit_text" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(120deg) translate(112px) rotate(-120deg);"><span>16</span></div>
        <div class="unit_text" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(180deg) translate(112px) rotate(-180deg);"><span>11</span></div>
        <div class="unit_text" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(240deg) translate(112px) rotate(-240deg);"><span>12</span></div>
        <div class="unit_text" style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(300deg) translate(112px) rotate(-300deg);"><span>13</span></div>
    </div>
    <script>
        function update(){
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "0",
                func:"rec_update_data"
            };
            // 使用 fetch 发送请求
            fetch('/data', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(dataToSend)
            })
            .then(response => response.json())
            .then(data => {

            })
            .catch(error => {
                console.error('Error:', error);
            });
        }
        function test(){
            // 选择所有包含 'circle' 类的元素
            const circles = document.querySelectorAll('.circle');
            
            // 使用 map 方法创建一个包含圆元素和数字部分的对象数组
            const circlesArray = Array.from(circles).map(circle => {
                const match = circle.className.match(/\b(\d+)\b/);
                return {
                    element: circle,
                    number: match ? parseInt(match[1], 10) : null
                };
            });
            
            // 对数组进行排序，基于数字部分
            circlesArray.sort((a, b) => a.number - b.number);

            const circleArray=circlesArray.map(item => item.element);
            
            for(i=1;i<=12;i++){
                circleArray[i].style.backgroundColor="blue";
            }

            
            

        }
    </script>
</body>
</html>
)rawliteral";