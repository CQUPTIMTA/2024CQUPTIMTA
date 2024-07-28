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
                center;
            align-items: center;
            font-size: 16;
            color: black;
        }

        .unit_text {
            position: absolute;
            display: flex;
            justify-content:
                center;
            align-items: center;
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

        .rectangle {
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
        <button onclick="window.location.href='/test'">测试</button>
        <button onclick="window.location.href='/cali'">校准</button>
    </div>
    <div class="data">
        <div class="unit">
            <label for="ID-11" class="label" data-id="11">ID: 11</label>
            <div>D：<span id="RUdataF1"></span>mm</div>
            <div>D：<span id="RUdataB1"></span>mm</div>
            <div>V：<span id="Voltage1"></span></div>
        </div>
        <div class="unit">
            <label for="ID-12" class="label" data-id="12">ID: 12</label>
            <div>D：<span id="RUdataF2"></span>mm</div>
            <div>D：<span id="RUdataB2"></span>mm</div>
            <div>V：<span id="Voltage2"></span></div>
        </div>
        <div class="unit">
            <label for="ID-13" class="label" data-id="13">ID: 13</label>
            <div>D：<span id="RUdataF3"></span>mm</div>
            <div>D：<span id="RUdataB3"></span>mm</div>
            <div>V：<span id="Voltage3"></span></div>
        </div>
        <div class="unit">
            <label for="ID-14" class="label" data-id="14">ID: 14</label>
            <div>D：<span id="RUdataF4"></span>mm</div>
            <div>D：<span id="RUdataB4"></span>mm</div>
            <div>V：<span id="Voltage4"></span></div>
        </div>
        <div class="unit">
            <label for="ID-15" class="label" data-id="15">ID: 15</label>
            <div>D：<span id="RUdataF5"></span>mm</div>
            <div>D：<span id="RUdataB5"></span>mm</div>
            <div>V：<span id="Voltage5"></span></div>
        </div>
        <div class="unit">
            <label for="ID-16" class="label" data-id="16">ID: 16</label>
            <div>D：<span id="RUdataF6"></span>mm</div>
            <div>D：<span id="RUdataB6"></span>mm</div>
            <div>V：<span id="Voltage6"></span></div>
        </div>
    </div>

    <div class="container">
        <div class="rectangle" style="top: 0; right: 0; transform: translate(-100px, 100px);"></div>
        <div class="rectangle" style="bottom: 0 ;right: 0; transform: translate(-100px, -100px);"></div>
        <div class="rectangle" style="top: 50%; left: 50%; transform: translate(-50%, -50%)"></div>
        <div class="circle zero-circle"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(0deg) translate(-250px) rotate(0deg);">
            <span>原点</span>
        </div>
        <!-- Inner circles -->
        <div class="circle 7"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(0deg) translate(75px) rotate(0deg);">
            <span>7</span>
        </div>
        <div class="circle 10"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(60deg) translate(75px) rotate(-60deg);">
            <span>10</span>
        </div>
        <div class="circle 9"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(120deg) translate(75px) rotate(-120deg);">
            <span>9</span>
        </div>
        <div class="circle 6"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(180deg) translate(75px) rotate(-180deg);">
            <span>6</span>
        </div>
        <div class="circle 3"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(240deg) translate(75px) rotate(-240deg);">
            <span>3</span>
        </div>
        <div class="circle 4"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(300deg) translate(75px) rotate(-300deg);">
            <span>4</span>
        </div>

        <!-- Outer circles -->
        <div class="circle 8"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(0deg) translate(150px) rotate(0deg);">
            <span>8</span>
        </div>
        <div class="circle 12"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(60deg) translate(150px) rotate(-60deg);">
            <span>12</span>
        </div>
        <div class="circle 11"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(120deg) translate(150px) rotate(-120deg);">
            <span>11</span>
        </div>
        <div class="circle 5"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(180deg) translate(150px) rotate(-180deg);">
            <span>5</span>
        </div>
        <div class="circle 1"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(240deg) translate(150px) rotate(-240deg);">
            <span>1</span>
        </div>
        <div class="circle 2"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(300deg) translate(150px) rotate(-300deg);">
            <span>2</span>
        </div>

        <!-- Outer circles -->
        <div class="unit_text"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(0deg) translate(112px) rotate(0deg);">
            <span>14</span>
        </div>
        <div class="unit_text"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(60deg) translate(112px) rotate(-60deg);">
            <span>15</span>
        </div>
        <div class="unit_text"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(120deg) translate(112px) rotate(-120deg);">
            <span>16</span>
        </div>
        <div class="unit_text"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(180deg) translate(112px) rotate(-180deg);">
            <span>11</span>
        </div>
        <div class="unit_text"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(240deg) translate(112px) rotate(-240deg);">
            <span>12</span>
        </div>
        <div class="unit_text"
            style="top: 50%; left: 50%; transform: translate(-50%, -50%) rotate(300deg) translate(112px) rotate(-300deg);">
            <span>13</span>
        </div>
    </div>
    <script>
        function update() {
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "0",
                func: "rec_update_data"
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
        function test() {
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

            const circleArray = circlesArray.map(item => item.element);

            for (i = 1; i <= 12; i++) {
                circleArray[i].style.backgroundColor = "blue";
            }

            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "0",
                func: "get_unit_data",
            };
            // 使用 fetch 发送请求
            fetch('../data', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(dataToSend)
            })

                .then(response => response.json())
                .then(data => {
                    // 更新 span 元素的文本内容，格式化到两位小数
                    RUdataF1.textContent = parseFloat(data.F1).toFixed(2);
                    RUdataB1.textContent = parseFloat(data.B1).toFixed(2);
                    Voltage1.textContent = parseFloat(data.V1).toFixed(2);

                    RUdataF2.textContent = parseFloat(data.F2).toFixed(2);
                    RUdataB2.textContent = parseFloat(data.B2).toFixed(2);
                    Voltage2.textContent = parseFloat(data.V2).toFixed(2);

                    RUdataF3.textContent = parseFloat(data.F3).toFixed(2);
                    RUdataB3.textContent = parseFloat(data.B3).toFixed(2);
                    Voltage3.textContent = parseFloat(data.V3).toFixed(2);

                    RUdataF4.textContent = parseFloat(data.F4).toFixed(2);
                    RUdataB4.textContent = parseFloat(data.B4).toFixed(2);
                    Voltage4.textContent = parseFloat(data.V4).toFixed(2);

                    RUdataF5.textContent = parseFloat(data.F5).toFixed(2);
                    RUdataB5.textContent = parseFloat(data.B5).toFixed(2);
                    Voltage5.textContent = parseFloat(data.V5).toFixed(2);

                    RUdataF6.textContent = parseFloat(data.F6).toFixed(2);
                    RUdataB6.textContent = parseFloat(data.B6).toFixed(2);
                    Voltage6.textContent = parseFloat(data.V6).toFixed(2);
                    const max_value = data.max;
                    const min_value = data.min;
                    var weights = [data.W1, data.W2, data.W3, data.W4, data.W5, data.W6, data.W7, data.W8, data.W9, data.W10, data.W11, data.W12];
                    const is_weight = (value) => {
                        if (value == "true") {
                            return true;
                        } else {
                            return false;
                        }
                    }
                    const set_color = (index, state) => {
                        if (state)
                            circleArray[index].style.backgroundColor = "#219ebc";
                        else {
                            circleArray[index].style.backgroundColor = "#white";
                        }
                    }
                    set_color(1, is_weight(weights[0]));
                    set_color(2, is_weight(weights[1]));
                    set_color(3, is_weight(weights[2]));
                    set_color(4, is_weight(weights[3]));
                    set_color(5, is_weight(weights[4]));
                    set_color(6, is_weight(weights[5]));
                    set_color(7, is_weight(weights[6]));
                    set_color(8, is_weight(weights[7]));
                    set_color(9, is_weight(weights[8]));
                    set_color(10, is_weight(weights[9]));
                    set_color(11, is_weight(weights[10]));
                    set_color(12, is_weight(weights[11]));

                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }
    </script>
</body>

</html>
)rawliteral";