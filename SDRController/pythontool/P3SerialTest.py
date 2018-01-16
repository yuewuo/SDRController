import serial

class Ser(object):
    def __init__(self):
        # 打开端口
        self.port = serial.Serial(port='COM9', baudrate=115200, bytesize=8, stopbits=1, timeout=10)

    # 发送指令的完整流程
    def send_cmd(self, cmd):
        self.port.write(cmd)
        response = self.port.readall()
        response = self.convert_hex(response)
        return response

    # 转成16进制的函数
    def convert_hex(self, string):
        res = []
        result = []
        for item in string:
            res.append(item)
        for i in res:
            result.append(hex(i))
        return result

    def receiveandsend(self):
        response = self.port.read()
        print(response)
        self.port.write(bytes("HTTP/1.1 200 OK\r\nContent-Length: 41\r\nConnection: keep-alive\r\n\
Content-Type: text/plain\r\n\r\nset time OK\r\nproxy at UART1 initialized\r\n", encoding='utf-8'))

if __name__ == '__main__':
    ser = Ser()
    ser.receiveandsend()
