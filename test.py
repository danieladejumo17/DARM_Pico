import smbus
import time


class PicoDevice:
    def __init__(self, channel=1):
        self.channel = channel
        self.bus = smbus.SMBus(channel)

    def trinaryToByte(self, trinary_digits):
        result = 0
        for i in range(len(trinary_digits)):
            result = (result*3) + trinary_digits[i]

        return result

    def trinary_to_bytes(self, trinary_array):
        # Convert trinary digits to decimal
        decimal_value = trinary_array[0] * 243 + \
            trinary_array[1] * 81 + trinary_array[2] * \
            27 + trinary_array[3] * 9 + trinary_array[4] * \
            3 + trinary_array[5]

        # Convert decimal value to bytes
        byte1 = decimal_value // 256
        byte2 = decimal_value % 256

        # Return array of two bytes
        return [byte1, byte2]

    def act(self, address, act_array):
        byte_data = self.trinary_to_bytes(act_array)
        try:
            # Write out I2C command: address, cmd, msg[0]
            self.bus.write_i2c_block_data(
                address, byte_data[0], [byte_data[1]])
        except Exception as e:
            print(f"Writing Error {str(e)}")
