#
# Copyright (C) 2007 by Johan De Taeye
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
#

# file : $URL$
# revision : $LastChangedRevision$  $LastChangedBy$
# date : $LastChangedDate$

from django.conf.urls.defaults import *

import common.views

urlpatterns = patterns('',

  # User preferences
  (r'^preferences/$', common.views.preferences),

  # Model list reports, which override standard admin screens
  (r'^admin/auth/user/$', 'common.report.view_report',
    {'report': common.views.UserList,}),
  (r'^admin/auth/group/$', 'common.report.view_report',
    {'report': common.views.GroupList,}),
  (r'^admin/common/dates/$', 'common.report.view_report',
    {'report': common.views.DatesList,}),

)