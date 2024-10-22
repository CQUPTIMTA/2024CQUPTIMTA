/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-06-12 16:01:20
 */
const char* testhtml = R"rawliteral(
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

        .main {
            display: flex;
            flex-direction: row;
            justify-content: left;
            align-items: flex-start;
            gap: 20px;
            /* 添加间距 */
            width: 70%;
            height: 100%;
            padding: 20px;
            box-sizing: border-box;
        }

        .main_button {
            /* 文字在中心显示 */
            font-size: 200px;
            display: center;
            flex-direction: row;
            justify-content: left;
            align-items: flex-start;
            /* 添加间距 */
            width: 100%;
            height: 100%;
            border-radius: 20%;
            box-sizing: border-box;
        }

        /* 触发 */
        .main_button:active {
            background-color: #174018;
        }
    </style>
</head>

<body>
    <div class="warp">
        <button onclick="window.location.href='/rec'">识别</button>
        <button onclick="window.location.href='/'">主页面</button>
        <button onclick="window.location.href='/cali'">校准</button>
    </div>
    <div class="container1">
        <button onclick="test_func()">测试</button>
        <button onclick="All_Z(true)">全部Z上升</button>
        <button onclick="All_Z(false)">全部Z下降</button>
        <button onclick="ALL_TO_START()">全部去起始点</button>
        <button onclick="ALL_TO_RELEASE()">全部去释放点</button>
    </div>
    <div class="container2">
        <button onclick="ALL_LASER(true)">全部激光开</button>
        <button onclick="ALL_LASER(false)">全部激光关</button>
        <button onclick="ALL_rezero('X')">全部X归零</button>
        <button onclick="ALL_rezero('Y')">全部Y归零</button>
        <button onclick="ALL_GRAP(true)">全部夹爪开</button>
        <button onclick="ALL_GRAP(false)">全部夹爪关</button>
    </div>
    <div class="main">
        <button class="main_button" onclick="MIAN_FUNC()">主程序</button>
    </div>

    <script>
        function test_func() {
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "1",
                func: "test_func"
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
        function All_Z(flag) {
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "1",
                func: "All_Z",
                state: flag
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

        function ALL_TO_START() {
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "1",
                func: "ALL_TO_START"
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

        function ALL_TO_RELEASE() {
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "1",
                func: "ALL_TO_RELEASE"
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

        function ALL_LASER(flag) {
            // 创建要发送的 JSON 数据   
            const dataToSend = {
                ID: "1",
                func: "ALL_LASER",
                state: flag
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

        function ALL_rezero(axis) {
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "1",
                func: "ALL_rezero",
                axis: axis
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

        function ALL_GRAP(flag) {
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "1",
                func: "ALL_GRAP",
                state: flag
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

        function MIAN_FUNC() {
            // 创建要发送的 JSON 数据
            const dataToSend = {
                ID: "1",
                func: "MIAN_FUNC"
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