/*
 * Copyright 2018 Alexander Stippich <a.stippich@gmx.net
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.3
import QtTest 1.0
import "../../src/qml"

FocusScope {

    property bool expandedFilterView: false

    function i18nc(string1,string2) {
        return string2
    }

    Item {
        id: elisaTheme
        property int layoutHorizontalMargin: 8
        property int layoutVerticalMargin: 6
        property int defaultFontPointSize: 12
        property int filterClearButtonMargin: layoutVerticalMargin
        property int ratingStarSize: 15
    }

    SystemPalette {
        id: myPalette
        colorGroup: SystemPalette.Active
    }

    Binding {
        target: navigationActionBar1
        property: "expandedFilterView"
        value: expandedFilterView
    }

    NavigationActionBar {
        id: navigationActionBar1

        mainTitle: 'testTitle1'
        secondaryTitle: 'secondaryTitle1'

        enableGoBack: true
        allowArtistNavigation: true
        showRating: true
        height: 100
    }

    NavigationActionBar {
        id: navigationActionBar2
        mainTitle: 'testTitle2'
        secondaryTitle: 'secondaryTitle2'

        enableGoBack: false
        allowArtistNavigation: false
        showRating: false
        expandedFilterView: true

        height: 100
        y: 200
    }

    TestCase {
        name: "TestNavigationActionBar"

        SignalSpy {
            id: enqueueSpy1
            target: navigationActionBar1
            signalName: "enqueue"
        }

        SignalSpy {
            id: enqueueSpy2
            target: navigationActionBar2
            signalName: "enqueue"
        }

        SignalSpy {
            id: replaceAndPlaySpy1
            target: navigationActionBar1
            signalName: "replaceAndPlay"
        }

        SignalSpy {
            id: replaceAndPlaySpy2
            target: navigationActionBar2
            signalName: "replaceAndPlay"
        }

        SignalSpy {
            id: goBackSpy1
            target: navigationActionBar1
            signalName: "goBack"
        }

        SignalSpy {
            id: goBackSpy2
            target: navigationActionBar2
            signalName: "goBack"
        }

        SignalSpy {
            id: filterViewChangedSpy1
            target: navigationActionBar1
            signalName: "filterViewChanged"
        }

        SignalSpy {
            id: filterViewChangedSpy2
            target: navigationActionBar2
            signalName: "filterViewChanged"
        }

        SignalSpy {
            id: showArtistSpy1
            target: navigationActionBar1
            signalName: "showArtist"
        }

        SignalSpy {
            id: showArtistSpy2
            target: navigationActionBar2
            signalName: "showArtist"
        }

        when: windowShown

        function init() {
            enqueueSpy1.clear();
            enqueueSpy2.clear();
            replaceAndPlaySpy1.clear();
            replaceAndPlaySpy2.clear();
            goBackSpy1.clear();
            goBackSpy2.clear();
            filterViewChangedSpy1.clear();
            filterViewChangedSpy2.clear();
            showArtistSpy1.clear();
            showArtistSpy2.clear();
            expandedFilterView = false;
        }

        function test_goBack() {
            compare(enqueueSpy1.count, 0);
            compare(enqueueSpy2.count, 0);
            compare(replaceAndPlaySpy1.count, 0);
            compare(replaceAndPlaySpy2.count, 0);
            compare(goBackSpy1.count, 0);
            compare(goBackSpy2.count, 0);
            compare(filterViewChangedSpy1.count, 0);
            compare(filterViewChangedSpy2.count, 0);
            compare(showArtistSpy1.count, 0);
            compare(showArtistSpy2.count, 0);

            var goPreviousButtonItem1 = findChild(navigationActionBar1, "goPreviousButton");
            verify(goPreviousButtonItem1 !== null, "valid goPreviousButton")
            mouseClick(goPreviousButtonItem1);
            compare(goBackSpy1.count, 1);
            var goPreviousButtonItem2 = findChild(navigationActionBar2, "goPreviousButton");
            verify(goPreviousButtonItem2 !== null, "valid goPreviousButton")
            mouseClick(goPreviousButtonItem2);
            compare(goBackSpy2.count, 0);
        }

        function test_enqueue() {
            compare(enqueueSpy1.count, 0);
            compare(enqueueSpy2.count, 0);
            compare(replaceAndPlaySpy1.count, 0);
            compare(replaceAndPlaySpy2.count, 0);
            compare(goBackSpy1.count, 0);
            compare(goBackSpy2.count, 0);
            compare(filterViewChangedSpy1.count, 0);
            compare(filterViewChangedSpy2.count, 0);
            compare(showArtistSpy1.count, 0);
            compare(showArtistSpy2.count, 0);

            var enqueueButtonItem = findChild(navigationActionBar1, "enqueueButton");
            verify(enqueueButtonItem !== null, "valid enqueueButton")
            mouseClick(enqueueButtonItem);
            compare(enqueueSpy1.count, 1);
        }

        function test_filterState() {
            compare(enqueueSpy1.count, 0);
            compare(enqueueSpy2.count, 0);
            compare(replaceAndPlaySpy1.count, 0);
            compare(replaceAndPlaySpy2.count, 0);
            compare(goBackSpy1.count, 0);
            compare(goBackSpy2.count, 0);
            compare(filterViewChangedSpy1.count, 0);
            compare(filterViewChangedSpy2.count, 0);
            compare(showArtistSpy1.count, 0);
            compare(showArtistSpy2.count, 0);

            var showFilterButtonItem1 = findChild(navigationActionBar1, "showFilterButton");
            verify(showFilterButtonItem1 !== null, "valid showFilterButton")
            mouseClick(showFilterButtonItem1);
            compare(filterViewChangedSpy1.count, 1);
            var signalArgument1 = filterViewChangedSpy1.signalArguments[0];
            expandedFilterView = signalArgument1[0];
            compare(expandedFilterView,true);
            compare(navigationActionBar1.state,'expanded')
            var showFilterButtonItem2 = findChild(navigationActionBar2, "showFilterButton");
            verify(showFilterButtonItem2 !== null, "valid showFilterButton")
            mouseClick(showFilterButtonItem2);
            compare(filterViewChangedSpy2.count, 1);
            var signalArgument2 = filterViewChangedSpy2.signalArguments[0];
            compare(signalArgument2[0],false)
            expandedFilterView = signalArgument2[0];
            compare(expandedFilterView,false)
            compare(navigationActionBar1.expandedFilterView, false)
            compare(navigationActionBar1.state,'collapsed')
        }

        function test_replaceAndPlay() {
            compare(enqueueSpy1.count, 0);
            compare(enqueueSpy2.count, 0);
            compare(replaceAndPlaySpy1.count, 0);
            compare(replaceAndPlaySpy2.count, 0);
            compare(goBackSpy1.count, 0);
            compare(goBackSpy2.count, 0);
            compare(filterViewChangedSpy1.count, 0);
            compare(filterViewChangedSpy2.count, 0);
            compare(showArtistSpy1.count, 0);
            compare(showArtistSpy2.count, 0);

            var replaceAndPlayButtonItem = findChild(navigationActionBar1, "replaceAndPlayButton");
            verify(replaceAndPlayButtonItem !== null, "valid replaceAndPlayButton")
            mouseClick(replaceAndPlayButtonItem);
            compare(replaceAndPlaySpy1.count, 1);
        }

        function test_showArtist() {
            compare(enqueueSpy1.count, 0);
            compare(enqueueSpy2.count, 0);
            compare(replaceAndPlaySpy1.count, 0);
            compare(replaceAndPlaySpy2.count, 0);
            compare(goBackSpy1.count, 0);
            compare(goBackSpy2.count, 0);
            compare(filterViewChangedSpy1.count, 0);
            compare(filterViewChangedSpy2.count, 0);
            compare(showArtistSpy1.count, 0);
            compare(showArtistSpy2.count, 0);

            var showArtistButtonItem1 = findChild(navigationActionBar1, "showArtistButton");
            verify(showArtistButtonItem1 !== null, "valid showArtistButton")
            mouseClick(showArtistButtonItem1);
            compare(showArtistSpy1.count, 1);
            var showArtistButtonItem2 = findChild(navigationActionBar2, "showArtistButton");
            verify(showArtistButtonItem2 !== null, "valid showArtistButton")
            mouseClick(showArtistButtonItem2);
            compare(showArtistSpy2.count, 0);
        }

        function test_filterRating() {
            expandedFilterView = true;
            wait(200);
            var ratingFilterItem1 = findChild(navigationActionBar1, "ratingFilter");
            verify(ratingFilterItem1 !== null, "valid ratingFilter")
            mouseClick(ratingFilterItem1,1);
            compare(navigationActionBar1.filterRating, 2);
            mouseClick(ratingFilterItem1,1);
            compare(navigationActionBar1.filterRating, 0);
            mouseClick(ratingFilterItem1,1 + elisaTheme.ratingStarSize);
            compare(navigationActionBar1.filterRating, 4);
            mouseClick(ratingFilterItem1,1 + elisaTheme.ratingStarSize);
            compare(navigationActionBar1.filterRating, 0);
            mouseClick(ratingFilterItem1,1 + 2 * elisaTheme.ratingStarSize);
            compare(navigationActionBar1.filterRating, 6);
            mouseClick(ratingFilterItem1,1 + 2 * elisaTheme.ratingStarSize);
            compare(navigationActionBar1.filterRating, 0);
            mouseClick(ratingFilterItem1,1 + 3 * elisaTheme.ratingStarSize);
            compare(navigationActionBar1.filterRating, 8);
            mouseClick(ratingFilterItem1,1 + 3 * elisaTheme.ratingStarSize);
            compare(navigationActionBar1.filterRating, 0);
            mouseClick(ratingFilterItem1,1 + 4 * elisaTheme.ratingStarSize);
            compare(navigationActionBar1.filterRating, 10);
            mouseClick(ratingFilterItem1,1 + 4 * elisaTheme.ratingStarSize);
            compare(navigationActionBar1.filterRating, 0);

            var ratingFilterItem2 = findChild(navigationActionBar2, "ratingFilter");
            verify(ratingFilterItem2 !== null, "valid ratingFilter")
            mouseClick(ratingFilterItem2,1);
            compare(navigationActionBar2.filterRating, 0);
            mouseClick(ratingFilterItem2,1 + elisaTheme.ratingStarSize);
            compare(navigationActionBar2.filterRating, 0);
            mouseClick(ratingFilterItem2,1 + 2 * elisaTheme.ratingStarSize);
            compare(navigationActionBar2.filterRating, 0);
            mouseClick(ratingFilterItem2,1 + 3 * elisaTheme.ratingStarSize);
            compare(navigationActionBar2.filterRating, 0);
            mouseClick(ratingFilterItem2,1 + 4 * elisaTheme.ratingStarSize);
            compare(navigationActionBar2.filterRating, 0);
        }

        function test_filterText() {
            var textsFilterItem1 = findChild(navigationActionBar1, "filterTextInput");
            verify(textsFilterItem1 !== null, "valid filterTextInput")
            compare(textsFilterItem1.focus, false);
            mouseClick(textsFilterItem1);
            compare(textsFilterItem1.focus, true);
            keyClick(Qt.Key_T);
            keyClick(Qt.Key_E);
            keyClick(Qt.Key_S);
            keyClick(Qt.Key_T);
            compare(navigationActionBar1.filterText, 'test');
            mouseClick(textsFilterItem1,textsFilterItem1.width - textsFilterItem1.height);
            compare(navigationActionBar1.filterText, "");
        }
    }
}
