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
            justify-content: left;
            align-items: center;
            height: 100vh;
            flex-direction:row
        }


    </style>
</head>
<body>
    <div class="warp">
        <button onclick="window.location.href='/'">主页面</button>
        <button onclick="window.location.href='/func'">测试</button>
    </div>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>
    <label for="ID-1" class="dynamic-label" data-id="1">ID: 1</label>

    <script>

    </script>
</body>
</html>
)rawliteral";