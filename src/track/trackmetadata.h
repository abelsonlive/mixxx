#pragma once

#include <QDateTime>

#include "track/albuminfo.h"
#include "track/trackinfo.h"


namespace mixxx {

class TrackMetadata final {
    // Audio properties
    //   - read-only
    //   - stored file tags
    //   - adjusted by audio decoder AFTER import from file tags
    PROPERTY_SET_BYVAL_GET_BYREF(AudioSource::Bitrate,      bitrate,    Bitrate)
    PROPERTY_SET_BYVAL_GET_BYREF(AudioSignal::ChannelCount, channels,   Channels)
    PROPERTY_SET_BYVAL_GET_BYREF(Duration,                  duration,   Duration)
    PROPERTY_SET_BYVAL_GET_BYREF(AudioSignal::SampleRate,   sampleRate, SampleRate)

    // Track properties
    //   - read-write
    //   - stored in file tags
    PROPERTY_SET_BYVAL_GET_BYREF(AlbumInfo, albumInfo, AlbumInfo)
    PROPERTY_SET_BYVAL_GET_BYREF(TrackInfo, trackInfo, TrackInfo)

public:
    TrackMetadata() = default;
    TrackMetadata(TrackMetadata&&) = default;
    TrackMetadata(const TrackMetadata&) = default;
    /*non-virtual*/ ~TrackMetadata() = default;

    TrackMetadata& operator=(TrackMetadata&&) = default;
    TrackMetadata& operator=(const TrackMetadata&) = default;

    // TODO(XXX): Remove after all new fields have been added to the library
    void resetUnsupportedValues() {
        refAlbumInfo().resetUnsupportedValues();
        refTrackInfo().resetUnsupportedValues();
    }

    // Adjusts floating-point values to match their string representation
    // in file tags to account for rounding errors.
    void normalizeBeforeExport() {
        refAlbumInfo().normalizeBeforeExport();
        refTrackInfo().normalizeBeforeExport();
    }

    // Compares the contents with metadata that has been freshly imported
    // from a file.
    bool hasBeenModifiedAfterImport(const TrackMetadata& importedFromFile) const {
        // NOTE(uklotzde): The read-only audio properties might differ after
        // they have been updated while decoding audio data. They are read-only
        // and must not be considered when exporting metadata.
        return (getAlbumInfo() != importedFromFile.getAlbumInfo()) ||
                (getTrackInfo() != importedFromFile.getTrackInfo());
    }

    // Parse an format date/time values according to ISO 8601
    static QDate parseDate(QString str) {
        return QDate::fromString(str.trimmed().replace(" ", ""), Qt::ISODate);
    }
    static QDateTime parseDateTime(QString str) {
        return QDateTime::fromString(str.trimmed().replace(" ", ""), Qt::ISODate);
    }
    static QString formatDate(QDate date) {
        return date.toString(Qt::ISODate);
    }
    static QString formatDateTime(QDateTime dateTime) {
        return dateTime.toString(Qt::ISODate);
    }

    // Parse and format the calendar year (for simplified display)
    static constexpr int kCalendarYearInvalid = 0;
    static int parseCalendarYear(QString year, bool* pValid = nullptr);
    static QString formatCalendarYear(QString year, bool* pValid = nullptr);

    static QString reformatYear(QString year);
};

bool operator==(const TrackMetadata& lhs, const TrackMetadata& rhs);

inline
bool operator!=(const TrackMetadata& lhs, const TrackMetadata& rhs) {
    return !(lhs == rhs);
}

QDebug operator<<(QDebug dbg, const TrackMetadata& arg);

} // namespace mixxx

Q_DECLARE_METATYPE(mixxx::TrackMetadata)
