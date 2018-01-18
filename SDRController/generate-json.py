
doc = """
{
    "ESP32": [
        {
            "addr": "192.168.31.37",
            "bootscript": "",
            "id": 1,
            "name": "Wuyue's ESP32",
            "port": 80
        }
    ],
    "GPIOExts": [
        {
            "bind": [
                "ESP32",
                1,
                "UART1"
            ],
            "id": 1
        }
    ],
    "plugins": [
        %s
    ],
    "switches": [
      %s
    ],
    "update": "2018/01/17 22:46:19",
    "version": "SDRController V0.1"
}

"""
charging_plugin = """
        {
            "bind": [
                "ESP32",
                1,
                "GPIO17"
            ],
            "descriptor": "Qi Wireless Charging",
            "name": "QiDemo",
            "position": "%s"
        }"""

nfc_plugin = """
        {
            "bind": [
                "ESP32",
                1,
                "UART2"
            ],
            "descriptor": "NFC Demo Module",
            "name": "PN532",
            "position": "%s"
        }"""

plugins = ""

swithches = ""

plugins += nfc_plugin % "0 0" + ',\n'
plugins += nfc_plugin % "0 10" + ',\n'
plugins += charging_plugin % "10 0" + ',\n'
plugins += charging_plugin % "10 9" + '\n'

cnt = -1
n = 10
m = 10
def link(x1, y1, x2, y2, isend=False):
  global cnt
  cnt = cnt + 1
  if isend:
    return '"' + str(x1) + ' ' + str(y1) + ' '  + str(x2) + ' ' + str(y2) + ' 1 A' + str(cnt) + '"\n'
  else:
    return '"' + str(x1) + ' ' + str(y1) + ' '  + str(x2) + ' ' + str(y2) + ' 1 A' + str(cnt) + '",\n'
for i in range(n):
  for j in range(m):
    swithches += link(i, j, i+1, j)
    swithches += link(i, j, i, j+1)
    swithches += link(i+1, j, i, j+1, i==n-1 and j==m-1)
    
with open("switches.json", "w") as f:
  f.write(doc % (plugins, swithches))
