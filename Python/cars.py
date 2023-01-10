import os
import csv

class CarBase:
    i = 0
    def __init__(self, brand, photo_file_name, carrying):
        self.i += 1
        self.brand = brand
        self.photo_file_name = photo_file_name
        self.carrying = float(carrying)

    def get_photo_file_ext(self):
        return os.path.splitext(self.photo_file_name)[1]

    @staticmethod
    def get_created_instances_count():
        return i



class Car(CarBase):
    def __init__(self, brand, photo_file_name, carrying, passenger_seats_count):
        super().__init__(brand, photo_file_name, carrying)
        self.car_type = "car"
        self.passenger_seats_count = int(passenger_seats_count)


class Truck(CarBase):
    def __init__(self, brand, photo_file_name, carrying, body_lwh):
        super().__init__(brand, photo_file_name, carrying)
        self.car_type = "truck"
        self.body_lwh = body_lwh

        gabs = body_lwh.split('x')
        try:
            if (len(gabs) == 3 and float(gabs[0]) >= 0 and float(gabs[1]) >= 0 and float(gabs[2]) >= 0):
                pass
            else:
                self.body_lwh = "0.0x0.0x0.0"
        except:
            self.body_lwh = "0.0x0.0x0.0"
        gabs = self.body_lwh.split('x')
        self.body_length = float(gabs[0])
        self.body_width = float(gabs[1])
        self.body_height = float(gabs[2])


    def get_body_volume(self):
            return self.body_length * self.body_width * self.body_height




class SpecMachine(CarBase):
    def __init__(self, brand, photo_file_name, carrying, extra):
        super().__init__(brand, photo_file_name, carrying)
        self.car_type = "spec_machine"
        self.extra = extra



def get_car_list(csv_filename):
    car_list = []
    with open(csv_filename) as csv_fd:
        reader = csv.reader(csv_fd, delimiter=';')
        next(reader)
        for row in reader:
            try:
                try:
                    if row[1] == '':
                        continue
                except:
                    continue
                try:
                    if os.path.splitext(row[3])[1] == '' or '.' in os.path.splitext(row[3])[0]:
                        continue
                except:
                    continue
                try:
                    float(row[5])
                    if float(row[5]) < 0:
                        continue
                except:
                    continue
                if row[0] == 'car':
                    try:
                        if (int(row[2]) < 0 or float(row[2]) % 1) > 0:
                            continue
                    except:
                        continue
                elif row[0] == 'truck':
                    try:
                        row[4]
                    except:
                        row[4] = "0.0x0.0x0.0"
                elif row[0] == 'spec_machine':
                    try:
                        if row[6] == '':
                            continue
                    except:
                        continue
                else:
                    continue
            except:
                continue
            if row[0] == 'car':
                car_list.append(Car(row[1], row[3], row[5], row[2]))
            elif row[0] == 'truck':
                car_list.append(Truck(row[1], row[3], row[5], row[4]))
            elif row[0] == 'spec_machine':
                car_list.append(SpecMachine(row[1], row[3], row[5], row[6]))

    return car_list
