# Serial Transmitter software designed in the ISL for Spiri Robotics

Currently broadcasts continuously

## Troubleshooting

### errno = 13: Permission Denied

User does not have access to the requested Serial Port. This can be fixed by changing this Serial Port's permissions with chmod:

```
sudo chmod 777 /dev/ttys0
```