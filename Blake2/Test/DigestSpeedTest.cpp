#include "DigestSpeedTest.h"

#include "../Blake2/BlakeB512.h"
#include "../BlakeC/blake2.h"

namespace BlakeTest
{
	void DigestSpeedTest::CBlake2BLoop(size_t SampleSize, size_t Loops)
	{
		std::vector<uint8_t> hash(64, 0);
		std::vector<uint8_t> buffer(128, 0);
		std::vector<uint8_t> key(0, 0);
		const char* name = "Blake2b";
		uint64_t start = TestUtils::GetTimeMs64();
		blake2b_state S[1];
		blake2b_init(S, 64);

		for (size_t i = 0; i < Loops; ++i)
		{
			size_t counter = 0;
			uint64_t lstart = TestUtils::GetTimeMs64();

			while (counter < SampleSize)
			{
				blake2b_update(S, (const uint8_t*)buffer.data(), buffer.size());
				counter += buffer.size();
			}

			blake2b_final(S, hash.data(), hash.size());
			std::string calc = IntToString((TestUtils::GetTimeMs64() - lstart) / 1000.0);
			OnProgress(const_cast<char*>(calc.c_str()));
		}

		uint64_t dur = TestUtils::GetTimeMs64() - start;
		uint64_t len = Loops * SampleSize;
		uint64_t rate = GetBytesPerSecond(dur, len);
		std::string glen = IntToString(len / GB1);
		std::string mbps = IntToString((rate / MB1));
		std::string secs = IntToString((double)dur / 1000.0);
		std::string resp = std::string(glen + "GB in " + secs + " seconds, avg. " + mbps + " MB per Second");

		OnProgress(const_cast<char*>(resp.c_str()));
		OnProgress("");
	}

	void DigestSpeedTest::CBlake2BPLoop(size_t SampleSize, size_t Loops)
	{
		std::vector<uint8_t> hash(64, 0);
		std::vector<uint8_t> buffer(SampleSize / 4, 0);
		std::vector<uint8_t> key(0, 0);
		const char* name = "Blake2bp";
		uint64_t start = TestUtils::GetTimeMs64();
		blake2bp_state S[1];
		blake2bp_init(S, 64);

		for (size_t i = 0; i < Loops; ++i)
		{
			size_t counter = 0;
			uint64_t lstart = TestUtils::GetTimeMs64();

			while (counter < SampleSize)
			{
				blake2bp_update(S, (const uint8_t*)buffer.data(), buffer.size());
				counter += buffer.size();
			}

			blake2bp_final(S, hash.data(), hash.size());
			std::string calc = IntToString((TestUtils::GetTimeMs64() - lstart) / 1000.0);
			OnProgress(const_cast<char*>(calc.c_str()));
		}

		uint64_t dur = TestUtils::GetTimeMs64() - start;
		uint64_t len = Loops * SampleSize;
		uint64_t rate = GetBytesPerSecond(dur, len);
		std::string glen = IntToString(len / GB1);
		std::string mbps = IntToString((rate / MB1));
		std::string secs = IntToString((double)dur / 1000.0);
		std::string resp = std::string(glen + "GB in " + secs + " seconds, avg. " + mbps + " MB per Second");

		OnProgress(const_cast<char*>(resp.c_str()));
		OnProgress("");
	}

	void DigestSpeedTest::CppBlake2BLoop(size_t SampleSize, size_t Loops, bool Parallel)
	{
		Blake2::BlakeB512 dgt(Parallel);
		size_t bufLen = Parallel ? SampleSize / 4 : 128;
		if (Parallel)
			dgt.ParallelBlockSize() = bufLen;
		std::vector<uint8_t> hash(dgt.DigestSize(), 0);
		std::vector<uint8_t> buffer(bufLen, 0);
		const char* name = dgt.Name();
		uint64_t start = TestUtils::GetTimeMs64();

		for (size_t i = 0; i < Loops; ++i)
		{
			size_t counter = 0;
			uint64_t lstart = TestUtils::GetTimeMs64();

			while (counter < SampleSize)
			{
				dgt.BlockUpdate(buffer, 0, buffer.size());
				counter += buffer.size();
			}

			dgt.DoFinal(hash, 0);
			std::string calc = IntToString((TestUtils::GetTimeMs64() - lstart) / 1000.0);
			OnProgress(const_cast<char*>(calc.c_str()));
		}

		uint64_t dur = TestUtils::GetTimeMs64() - start;
		uint64_t len = Loops * SampleSize;
		uint64_t rate = GetBytesPerSecond(dur, len);
		std::string glen = IntToString(len / GB1);
		std::string mbps = IntToString((rate / MB1));
		std::string secs = IntToString((double)dur / 1000.0);
		std::string resp = std::string(glen + "GB in " + secs + " seconds, avg. " + mbps + " MB per Second");

		OnProgress(const_cast<char*>(resp.c_str()));
		OnProgress("");
	}

	uint64_t DigestSpeedTest::GetBytesPerSecond(uint64_t DurationTicks, uint64_t DataSize)
	{
		double sec = (double)DurationTicks / 1000.0;
		double sze = (double)DataSize;

		return (uint64_t)(sze / sec);
	}

	void DigestSpeedTest::OnProgress(char* Data)
	{
		m_progressEvent(Data);
	}
}