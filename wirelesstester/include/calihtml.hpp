const char* calihtml = R"rawliteral(
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
            justify-content: left;
            align-items: center;
            height: 100vh;
            flex-direction: row
        }

        button {
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 10px;
            cursor: pointer;
            margin-bottom: 10px;
            width: 150px;
            font-size: 16px;
            height: 10vh;
        }

        button:hover {
            background-color: #45a049;
        }

        input[type="number"] {
            width: 40%;
            padding: 5px;
            border: 1px solid #ccc;
            border-radius: 5px;
            font-size: large;
            text-align: center;
        }

        .warp {
            display: flex;
            flex-direction: column;
            justify-content: left;
            align-items: flex-start;
            gap: 20px;
            /* 添加间距 */
            width: 20%;
            padding: 20px;

            box-sizing: border-box;
        }

        .container1 {
            display: flex;
            flex-direction: column;
            justify-content: left;
            align-items: flex-start;
            gap: 20px;
            /* 添加间距 */
            width: 20%;
            padding: 20px;
            box-sizing: border-box;
        }

        .container2 {
            display: flex;
            flex-direction: column;
            justify-content: left;
            align-items: flex-start;
            gap: 20px;
            /* 添加间距 */
            width: 20%;
            padding: 20px;
            box-sizing: border-box;
        }

        .container3 {
            display: flex;
            flex-direction: column;
            justify-content: left;
            align-items: flex-start;
            gap: 20px;
            /* 添加间距 */
            width: 20%;
            padding: 20px;
            box-sizing: border-box;
        }
    </style>
</head>

<body>
    <div class="warp">
        <button onclick="window.location.href='/rec'">识别</button>
        <button onclick="window.location.href='/'">主页面</button>
        <button onclick="window.location.href='/test'">测试</button>
    </div>
    <div class="container1">
        <label for="ID">抓取单元ID:</label>
        <input type="number" id="ID" value="1" min="1" max="5" step="1">
        <button onclick="move(document.getElementById('ID').value, -1)">-1mm</button>
        <button onclick="move(document.getElementById('ID').value, -5)">-5mm</button>
        <button onclick="cali(document.getElementById('ID').value)">设置</button>
        <button onclick="move(document.getElementById('ID').value, 5)">+5mm</button>
        <button onclick="move(document.getElementById('ID').value, 1)">+1mm</button>
    </div>
    <div class="container2">
        <label for="ID">横梁单元ID:</label>
        <input type="number" id="CID" value="6" min="6" max="8" step="1">
        <button onclick="move(document.getElementById('CID').value, -1)">-1mm</button>
        <button onclick="move(document.getElementById('CID').value, -5)">-5mm</button>
        <button onclick="cali(document.getElementById('CID').value)">设置</button>
        <button onclick="move(document.getElementById('CID').value, 5)">+5mm</button>
        <button onclick="move(document.getElementById('CID').value, 1)">+1mm</button>
    </div>
    <div class="container3">
        <label for="ID">砝码修正:</label>
        <button onclick="">移动到校准位置</button>
        <button onclick="">X+</button>
        <button onclick="">X-</button>
        <button onclick="">设置</button>
        <button onclick="">Y+</button>
        <button onclick="">Y-</button>
    </div>

    <script>
        function move(ID, _value) {

            const id = ID;
            var _axis;
            if (id < 6) {
                _axis = "X";
            } else {
                _axis = "Y";
            }
            const __value = _value;
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func: "move_axis",
                axis: _axis,
                value: __value
            };
            // 使用 fetch 发送请求
            fetch('/data', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(dataToSend)
            })
        }
        function cali(ID) {
            const id = ID;
            //创建数组
            const values = [1755, 245, 1000, 1755, 1755, 245, 2750, 3755];
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func: "set_now",
                value: values[id - 1]
            };
            // 使用 fetch 发送请求
            fetch('/data', {

                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(dataToSend)
            })
        }
    </script>
</body>

</html>
)rawliteral";