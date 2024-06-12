const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
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
            flex-direction:row
        }

        .container {
            display: flex;
            flex-direction: row; /* 横向排列 */
            justify-content: left;
            align-items: flex-start;
            gap: 20px; /* 添加间距 */
            width: 93%;
            padding: 20px;
            box-sizing: border-box;
        }

        .grap,
        .crossbeam {
            background-color: #ffffff;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            padding: 20px;
            text-align: left;
            height: 90vh;
            width: 300px; /* 设置固定宽度 */
        }

        input[type="number"] {
            width: calc(100% - 22px);
            padding: 5px;
            border: 1px solid #ccc;
            border-radius: 5px;
            font-size: large;
            text-align: center;
        }

        button {
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 0px;
            cursor: pointer;
            margin: 3px 0;
            width: 100%;
            font-size: 16px;
            height: 6%;
        }

        button:hover {
            background-color: #45a049;
        }

        span {
            font-weight: bold;
            color: #333;
        }

        .crossbeam label,
        .grap label {
            font-size: large;
            color: #333;
            font-weight: bold;
            margin: 0;
        }
        .warp {
            display: flex;
            flex-direction: column;
            justify-content: flex-start; /* 对齐到顶部 */
            gap: 10px; /* 增加按钮之间的间距 */
            height: 30%;
            width: 100px; /* 调整容器宽度以适应按钮文本 */
        }
        .warp button {
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 10px; /* 增加内边距 */
            cursor: pointer;
            margin: 5px 0; /* 增加外边距 */
            width: 100%;
            font-size: 16px;
            height: auto;
        }
    </style>
</head>
<body>
    <div class="warp">
        <button onclick="window.location.href='/rec'">识别</button>
        <button onclick="window.location.href='/test'">测试</button>
    </div>
    <div class="container">
        <div class="grap">
            <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
            <button onclick="grap_update_data(this)">更新数据</button>
            <button onclick="rezero(this)">归零</button>
            <button onclick="load(this,false)">卸载</button>
            <button onclick="load(this,true)">使能</button>
            <input type="number" id="Zvalue" value="230" min="0" max="250" step="10">
            <button onclick="move_to_axis(this,'Z')">移动到Z</button>
            <input type="number" id="Xvalue" value="250" min="0" max="1800" step="50">
            <button onclick="move_to_axis(this,'X')">移动到X</button>
            <div>Z：<span id="Zposition"></span></div>
            <div>X：<span id="Xposition"></span></div>
            <div>T：<span id="servo_temp"></span>C°</div>
            <button onclick="grap(this,true)">抓取</button>
            <button onclick="grap(this,false)">放开</button>
            <button onclick="laser(this,true)">激光开</button>
            <button onclick="laser(this,false)">激光关</button>
        </div>
    </div>

    <script>
        document.addEventListener("DOMContentLoaded", function() {
            const grapContainer = document.querySelector('.container');
            const grap = document.querySelector('.grap');

            // Clone grap elements
            for (let i = 2; i <= 5; i++) { // Start from 2 as 1 is already present
                const clone = grap.cloneNode(true);

                // Update ID and label for grap
                const label = clone.querySelector('.dynamic-label');
                label.textContent = `ID: ${i}`;
                const newID = `ID-${i}`;
                label.setAttribute('for', newID);
                label.setAttribute("data-id",i);
                grapContainer.appendChild(clone);
            }

            // Clone crossbeam elements
            for (let i = 6; i <= 8; i++) { // Clone crossbeam three times
                const clone = document.createElement('div');
                clone.classList.add('crossbeam');

                // Set the inner HTML for the crossbeam
                clone.innerHTML = `
                    <label for="crossbeam-ID-${i}" class="dynamic-label2" data-id="${i}">ID: ${i}</label>
                    <button onclick="crossbeam_update_data(this)">更新数据</button>
                    <button onclick="rezero(this)">归零</button>
                    <button onclick="load(this,false)">卸载</button>
                    <button onclick="load(this,true)">使能</button>
                    <input type="number" id="Yvalue" value="1000" min="0" max="3800" step="50">
                    <button onclick="move_to_axis(this,'Y')">移动到Y</button>
                    <div>Y：<span id="Yposition"></span></div>
                    <div>V：<span id="Voltage"></span></div>
                `;

                grapContainer.appendChild(clone);
            }
        });
        function grap(button,_state){
            // 获取当前按钮所属的label元素
            const label = button.parentNode.querySelector('label');
            // 从label元素中读取ID值
            const id = label.getAttribute('data-id');
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func:"grap",
                state:_state
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
        function grap_update_data(button){
            // 获取当前按钮所属的父元素grap
            const grap = button.closest('.grap');
            // 获取当前按钮所属的label元素
            const label = button.parentNode.querySelector('label');
            // 从label元素中读取ID值
            const id = label.getAttribute('data-id');

            // 更新Z位置和X位置的span文本
            const zPositionSpan = grap.querySelector("#Zposition");
            const xPositionSpan = grap.querySelector("#Xposition");
            const servoTempSpan = grap.querySelector("#servo_temp");

            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func:"grap_update_data"
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
                // 更新 span 元素的文本内容
                zPositionSpan.textContent = data.Zposition;
                xPositionSpan.textContent = data.Xposition;
                servoTempSpan.textContent = data.servo_temp;
            })
            .catch(error => {
                console.error('Error:', error);
            });

        }
        function crossbeam_update_data(button){
            // 获取当前按钮所属的父元素
            const grap = button.closest('.crossbeam');
            // 获取当前按钮所属的label元素
            const label = button.parentNode.querySelector('label');
            // 从label元素中读取ID值
            const id = label.getAttribute('data-id');

            // 更新Z位置和X位置的span文本
            const YPositionSpan = grap.querySelector("#Yposition");
            const Voltage = grap.querySelector("#Voltage");

            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func:"crossbeam_update_data"
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
                // 更新 span 元素的文本内容
                YPositionSpan.textContent = data.YPositionSpan;
                Voltage.textContent = data.Voltage;

            })
            .catch(error => {
                console.error('Error:', error);
            });

        }
        function rezero(button){
            // 获取当前按钮所属的label元素
            const label = button.parentNode.querySelector('label');
            // 从label元素中读取ID值
            const id = label.getAttribute('data-id');
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func:"rezero"
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
        function load(button,_state){
            // 获取当前按钮所属的label元素
            const label = button.parentNode.querySelector('label');
            // 从label元素中读取ID值
            const id = label.getAttribute('data-id');
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func:"load",
                state:_state
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
        function move_to_axis(button,_axis){
            const valueInput = button.parentNode.querySelector(`#${_axis}value`);
            const _value = valueInput.value;
            // 获取当前按钮所属的label元素
            const label = button.parentNode.querySelector('label');
            // 从label元素中读取ID值
            const id = label.getAttribute('data-id');

            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func:"move_to_axis",
                axis:_axis,
                value:_value
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
        function laser(button,_state){
            // 获取当前按钮所属的label元素
            const label = button.parentNode.querySelector('label');
            // 从label元素中读取ID值
            const id = label.getAttribute('data-id');
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: id,
                func:"laser",
                state:_state
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