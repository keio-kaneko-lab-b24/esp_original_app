import subprocess
import asyncio
import time
import argparse
import glob

parser = argparse.ArgumentParser()
parser.add_argument('--taskType', type=str, default="tuning")
parser.add_argument('--taskNumber', type=int, default=1)
parser.add_argument('--onlyMonitor', action='store_true')
parser.add_argument('--flipImage', action='store_true')
args = parser.parse_args()


async def view_monitor(task_number, task_type):
    '''筋電をモニターするための画面を起動する'''
    print("start view_monitor")
    subprocess.Popen(
        f"python monitor/view_monitor.py --taskNumber {task_number} --taskType {task_type}", shell=True)
    print("end view_monitor")


async def do_monitor(task_number, task_type):
    '''
    ESPから信号処理後の筋電を継続的に取得して、monitor.txtへ書き込み続ける。
    '''
    print("start do_monitor")
    subprocess.call(
        f"python monitor/do_monitor.py --taskNumber {task_number} --taskType {task_type}", shell=True)
    print("end do_monitor")


def main():
    loop = asyncio.get_event_loop()

    if args.onlyMonitor:
        task_number = args.taskNumber
        loop.run_until_complete(asyncio.gather(
            *[view_monitor(999, "only_monitor"), do_monitor(999, "only_monitor")]))
    else:
        print("** ERROR **\nこのアプリは--onlyMonitorしかサポートしていません。")


if __name__ == '__main__':
    main()
