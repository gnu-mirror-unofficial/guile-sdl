# init.sed                                 -*- shell-script -*-

# Copyright (C) 2013 Thien-Thi Nguyen
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 3 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

s/SDL_INIT_TIMER/timer/g
s/SDL_INIT_AUDIO/audio/g
s/SDL_INIT_VIDEO/video/g
s/SDL_INIT_CDROM/cdrom/g
s/SDL_INIT_JOYSTICK/joystick/g
s/SDL_INIT_EVERYTHING/everything/g
s/SDL_INIT_NOPARACHUTE/no-parachute/g
s/SDL_INIT_EVENTTHREAD/event-thread/g

# init.sed ends here
